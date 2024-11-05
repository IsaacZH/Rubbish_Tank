/*
* chassis
*	
*	2023.9.11
* ����
*/

/* Includes ------------------------------*/
#include "chassis.h"
#include "device.h"
#include "rp_math.h"
#include "car.h"
#include "remote.h"
#include "gpio.h"
#include "drv_tim.h"
/* Private function prototypes -----------------------------------------------*/

void Chassis_Speed_Calculating(chassis_t *chassis);//���ֵ��̵��Ŀ���ٶȽ���

void Chassis_Mec_Update(chassis_t *chassis);//���̻�еģʽ���� 

void Chassis_Work_Mec(chassis_t *chassis);//��еģʽ

/* Exported variables --------------------*/
chassis_t chassis = 
{
	.move_dir = sideways_off,

};

/* Function  body --------------------------------------------------------*/

/**
 * @brief ���̻�еģʽ����
 * 
 * @param chassis 
 * @param ctrl_move 0:ң����ģʽ 1������ģʽ
 * @note ch3:��ң��Y�� ch2:��ң��X�� ch0:��ң��X��
 */
void Chassis_Mec_Update(chassis_t *chassis)
{
	if(chassis->move_dir == sideways_on)
	{
		chassis->base_info.target_front_speed = -(float) rc.base_info->ch2 / RC_CNT_MAX * CHASSIS_MAX_SPEED;

	}
	else
	{
		chassis->base_info.target_front_speed = (float) rc.base_info->ch3 / RC_CNT_MAX * CHASSIS_MAX_SPEED;
	}
	chassis->base_info.target_cycle_speed = (float) rc.base_info->ch0 / RC_CNT_MAX * CHASSIS_MAX_SPEED;
}


/**
  * @Name    Chassis_Speed_Calculating
  * @brief   ���̵��Ŀ���ٶȼ���
  * @param   
  * @retval
  * @author  HWX CCB Isaac 
  * @Date    2022-12-3
**/
void Chassis_Speed_Calculating(chassis_t *chassis)
{
	int16_t front = chassis->base_info.target_front_speed;
	int16_t cycle = chassis->base_info.target_cycle_speed;
	
		
	int16_t speed_sum;
	float K;
	
	speed_sum = abs(front) + abs(cycle);
	
	if(speed_sum > CHASSIS_MAX_SPEED)
	{
		K = (float)CHASSIS_MAX_SPEED / (float)speed_sum;
	}
	else 
	{
		K = 1;
	}

	front *= K;
	cycle *= K;
	
	chassis->base_info.target_chassis_L   =   front + cycle; 
	chassis->base_info.target_chassis_R   = - front + cycle; 
	
}


/**
  * @Name    
  * @brief   ���̻�еģʽ
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Work_Mec(chassis_t *chassis)
{
	Chassis_Mec_Update(chassis);//���̻�еģʽ����
	Chassis_Speed_Calculating(chassis);//���ֵ��̵��Ŀ���ٶȽ���
}



///**
// * @brief ��������ִ��
// * 
// * @param chassis 
// */
//void Chassis_Cmd_Excute(chassis_t *chassis)
//{
//	if(car.car_move_mode == offline_CAR || car.car_move_mode == init_CAR)
//	{
//		return;
//	}

//	if (chassis->cmd_switch_dir->cmd_value == true)
//	{
//		if (chassis->move_dir == sideways_off)
//		{
//			chassis->move_dir = sideways_on;
//		}
//		else
//		{
//			chassis->move_dir = sideways_off;
//		}
//	}
//}

/**
 * @brief �����ܿ� device�е���
 * 
 * @param chassis 
 */
void Chassis_Work(chassis_t *chassis)
{
	/*Ŀ��ֵ����********************* */
	Chassis_Work_Mec(chassis);
	
	/*������********************* */
	//TODO: ���ú�IO��
	//�ؿر���
	if (!RC_ONLINE)
	{
		//ж��
		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,GPIO_PIN_RESET);
	}
	else
	{
		//�ϵ�
		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,GPIO_PIN_SET);
		//�Ҳ�ת��
		if (chassis->base_info.target_chassis_R > 0)
		{
			HAL_GPIO_WritePin(DIR1_GPIO_Port,DIR1_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(DIR1_GPIO_Port,DIR1_Pin,GPIO_PIN_RESET);
		}
		//���ת��
		if (chassis->base_info.target_chassis_L > 0)
		{
			HAL_GPIO_WritePin(DIR2_GPIO_Port,DIR2_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(DIR2_GPIO_Port,DIR2_Pin,GPIO_PIN_RESET);
		}
		//ת�ٿ���
		float l_duty = abs(chassis->base_info.target_chassis_L) / CHASSIS_MAX_OUTPUT_SPEED * 100.f;
		float r_duty = abs(chassis->base_info.target_chassis_R) / CHASSIS_MAX_OUTPUT_SPEED * 100.f;
		Set_PWM_DutyCycle(TIM_CHANNEL_1, l_duty);
		Set_PWM_DutyCycle(TIM_CHANNEL_2, r_duty);
	}
		
}
