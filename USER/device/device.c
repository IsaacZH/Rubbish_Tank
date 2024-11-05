/**
  ******************************************************************************
  * @file           : device.c/h
  * @brief          : 
  * @note           :  
  ******************************************************************************
  */
#include "device.h"
#include "chassis.h"
#include "gimbal.h"
#include "shooting.h"
#include "image.h"
#include "led.h"
#include "laser.h"
device_t device [] =
{	
	[DEVICE_01] = {
	 .motor = &gimbal_motor[GIMBAL_YAW],	
	},
	[DEVICE_02] = {
	 .motor = &gimbal_motor[GIMBAL_PITCH],	
	},
	[DEVICE_03] = {
	 .motor = &shooting_motor[DAIL],	
	},
	[DEVICE_04] = {
	 .motor = &shooting_motor[LIMIT],	
	},
	[DEVICE_05]= {
	 .motor = &shooting_motor[FRICTION_LF],	
	},
	[DEVICE_06] = {
	 .motor = &shooting_motor[FRICTION_RF],	
	},
	[DEVICE_07] = {
	 .motor = &shooting_motor[FRICTION_LB],	
	},
	[DEVICE_08] = {
	 .motor = &shooting_motor[FRICTION_RB],	
	},
	[DEVICE_09] = {
	 .motor = &image_motor,	
	},
};
/**
  * @Name    Device_Init
  * @brief   设备初始化
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Device_Init(void)
{
	image_motor_init();
	gimbal_motor_init();
	shooting_motor_init();
}

/**
  * @Name    Device_HeartBeat
  * @brief   设备心跳
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Device_HeartBeat(void)
{
	image_motor_heart_beat();
	gimbal_motor_heart_beat();
	shooting_motor_heart_beat();
	//设备在线指示灯
	if ((!IMAGE_MOTOR_ONLINE || !GIMBAL_MOTOR_ONLINE || !SHOOTING_MOTOR_ONLINE))
	{
		led_blue.blink(&led_blue,500);
	}
	else if (communicate.status->work_state == DEV_OFFLINE)
	{
		led_blue.off(&led_blue);
	}
	else
	{
		led_blue.on(&led_blue);
	}
}

/**
  * @Name    Device_Work
  * @brief   设备总控，任务中调用
  * @param   
  * @retval 
  * @author  
  * @Date    
**/
void Device_Work(void)
{
  chassis.work(&chassis);
	gimbal.work(&gimbal);
	shooting.work(&shooting);
	image.work(&image);
	laser.work(&laser);
	
	if(!RC_ONLINE)
	{
		for(uint16_t i=0;i<DEVICE_CNT;i++)
		{
			device[i].motor->base_info.motor_out = 0;
			device[i].motor->base_info.pid_out = 0;
		}
	}
	
}

