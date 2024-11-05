#include "my_task.h"
#include "drive.h"
#include "gpio.h"
#include "drv_tim.h"
#include "remote.h"
/**
  * @Name    StartControlTask
  * @brief   控制任务
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
//extern IWDG_HandleTypeDef hiwdg;
float dutyCycle = 0;
GPIO_PinState EN2_PinState = GPIO_PIN_SET;
GPIO_PinState DIR2_PinState = GPIO_PIN_RESET;

void StartControlTask(void const * argument)
{
//	Device_Init();
	Drive_Init();
	RC_Init(&rc);				//device.h -> remote.h
  for(;;)
  {
//		Set_PWM_DutyCycle(TIM_CHANNEL_2, dutyCycle);
//		HAL_GPIO_WritePin(EN2_GPIO_Port,EN2_Pin,EN2_PinState);
//		HAL_GPIO_WritePin(DIR2_GPIO_Port,DIR2_Pin,DIR2_PinState);
		
		Set_PWM_DutyCycle(TIM_CHANNEL_3, dutyCycle);
		HAL_GPIO_WritePin(AIN1_GPIO_Port,AIN1_Pin,EN2_PinState);
		HAL_GPIO_WritePin(AIN2_GPIO_Port,AIN2_Pin,DIR2_PinState);


		osDelay(1);

  }
}

/**
  * @Name    StartRealTimeTask
  * @brief   实时任务更新
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void StartRealTimeTask(void const * argument)
{
  for(;;)
  {
		rc_ctrl(&rc);

//		HAL_IWDG_Refresh(&hiwdg);
    osDelay(1);
  }
}

/**
  * @Name    StartHeartBeatTask
  * @brief   监控任务
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void StartHeartBeatTask(void const * argument)
{
	HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
  for(;;)
  {
//    Cmd_Heartbeat();
//		Device_HeartBeat();
		rc_heart_beat(&rc);
//    Communicate_Heartbeat();

    osDelay(1);
  }
}



