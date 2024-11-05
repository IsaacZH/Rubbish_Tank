/*
* chassis
*	
*	2023.9.11
* 底盘
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

void Chassis_Speed_Calculating(chassis_t *chassis);//麦轮底盘电机目标速度解算

void Chassis_Mec_Update(chassis_t *chassis);//底盘机械模式更新 

void Chassis_Work_Mec(chassis_t *chassis);//机械模式

/* Exported variables --------------------*/
chassis_t chassis = 
{
	.move_dir = sideways_off,

	.work = Chassis_Work,
};

/* Function  body --------------------------------------------------------*/

/**
 * @brief 底盘机械模式更新
 * 
 * @param chassis 
 * @param ctrl_move 0:遥控器模式 1：键盘模式
 * @note ch3:左遥感Y轴 ch2:左遥感X轴 ch0:右遥感X轴
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
  * @brief   底盘电机目标速度计算
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
  * @brief   底盘机械模式
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Chassis_Work_Mec(chassis_t *chassis)
{
	Chassis_Mec_Update(chassis);//底盘机械模式更新
	Chassis_Speed_Calculating(chassis);//麦轮底盘电机目标速度解算
}



///**
// * @brief 底盘命令执行
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
 * @brief 底盘总控 device中调用
 * 
 * @param chassis 
 */
void Chassis_Work(chassis_t *chassis)
{
//	Chassis_Cmd_Excute(chassis);

//  switch(car.car_move_mode)
//	{
//		case offline_CAR:	
//			chassis->base_info.output_chassisLF = 0;
//			chassis->base_info.output_chassisRB = 0;
//			chassis->move_dir = sideways_off;
//			break;
//		case init_CAR:
//      chassis->base_info.target_cycle_speed = 0;
//      chassis->base_info.target_front_speed = 0;
//		break;
//		case mec_CAR:
//			Chassis_Work_Mec(chassis);
//			break;
//			
//		default:
//			break;
//	}
	
	Chassis_Work_Mec(chassis);
	//关控保护
	if (!RC_ONLINE)
	{
		//卸力
		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,GPIO_PIN_RESET);
	}
	else
	{
		//上电
		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(EN1_GPIO_Port,EN1_Pin,GPIO_PIN_SET);
		//右侧转向
		if (chassis->base_info.target_chassis_R > 0)
		{
			HAL_GPIO_WritePin(DIR1_GPIO_Port,DIR1_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(DIR1_GPIO_Port,DIR1_Pin,GPIO_PIN_RESET);
		}
		//左侧转向
		if (chassis->base_info.target_chassis_L > 0)
		{
			HAL_GPIO_WritePin(DIR2_GPIO_Port,DIR2_Pin,GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(DIR2_GPIO_Port,DIR2_Pin,GPIO_PIN_RESET);
		}
		//转速控制
		float l_duty = abs(chassis->base_info.target_chassis_L) / CHASSIS_MAX_SPEED;
		
	}


	
		
		
}
