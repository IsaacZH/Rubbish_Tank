#ifndef _CHASSIS_H
#define _CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "config.h"
/* Exported macro ------------------------------------------------------------*/
#define CHASSIS_MAX_SPEED						(10000.f) //6000

/** 
  * @brief  底盘基本信息定义
  */ 
typedef __packed struct  
{
	
	float      target_front_speed;//目标前进速度
	float      target_cycle_speed;//目标旋转速度
		
	float    	 target_chassis_L;
	float    	 target_chassis_R;

}chassis_base_info_t;

/**
 * @brief 底盘是否开启侧身
 * 
 */
typedef enum
{
	sideways_off = 0,
	sideways_on,
}chassis_move_direction_e;

/** 
  * @brief  底盘类定义
  */ 
typedef struct chassis_class_t 
{	
	
	chassis_move_direction_e move_dir;

	chassis_base_info_t base_info;
	
	void                (*work)(struct chassis_class_t *chassis);

}chassis_t;


extern chassis_t chassis;



/* Exported functions --------------------------------------------------------*/

//总控
void Chassis_Work(chassis_t *chassis);




#endif