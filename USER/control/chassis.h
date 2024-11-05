#ifndef _CHASSIS_H
#define _CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "communicate_protocol.h"
/* Exported macro ------------------------------------------------------------*/
#define CHASSIS_MAX_SPEED           (8000.f) 



/** 
  * @brief  ���̻�����Ϣ����
  */ 
typedef struct __packed 
{
	int16_t target_front_speed;//Ŀ��ǰ���ٶ�
	int16_t target_cycle_speed;//Ŀ����ת�ٶ�

}chassis_base_info_t;


/** 
  * @brief  �����ඨ��
  */ 
typedef struct chassis_class_t
{	

	chassis_base_info_t  base_info;
		
	void (*work)(struct chassis_class_t *chassis);

}chassis_t;


extern chassis_t chassis;



/* Exported functions --------------------------------------------------------*/





#endif
