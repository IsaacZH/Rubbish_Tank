#ifndef _CHASSIS_H
#define _CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "communicate_protocol.h"
/* Exported macro ------------------------------------------------------------*/
#define CHASSIS_MAX_SPEED           (8000.f) 



/** 
  * @brief  底盘基本信息定义
  */ 
typedef struct __packed 
{
	int16_t target_front_speed;//目标前进速度
	int16_t target_cycle_speed;//目标旋转速度

}chassis_base_info_t;


/** 
  * @brief  底盘类定义
  */ 
typedef struct chassis_class_t
{	

	chassis_base_info_t  base_info;
		
	void (*work)(struct chassis_class_t *chassis);

}chassis_t;


extern chassis_t chassis;



/* Exported functions --------------------------------------------------------*/





#endif
