#ifndef _CHASSIS_H
#define _CHASSIS_H

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "config.h"
/* Exported macro ------------------------------------------------------------*/
#define CHASSIS_MAX_SPEED						(10000.f) //6000

/** 
  * @brief  ���̻�����Ϣ����
  */ 
typedef __packed struct  
{
	
	float      target_front_speed;//Ŀ��ǰ���ٶ�
	float      target_cycle_speed;//Ŀ����ת�ٶ�
		
	float    	 target_chassis_L;
	float    	 target_chassis_R;

}chassis_base_info_t;

/**
 * @brief �����Ƿ�������
 * 
 */
typedef enum
{
	sideways_off = 0,
	sideways_on,
}chassis_move_direction_e;

/** 
  * @brief  �����ඨ��
  */ 
typedef struct chassis_class_t 
{	
	
	chassis_move_direction_e move_dir;

	chassis_base_info_t base_info;
	
	void                (*work)(struct chassis_class_t *chassis);

}chassis_t;


extern chassis_t chassis;



/* Exported functions --------------------------------------------------------*/

//�ܿ�
void Chassis_Work(chassis_t *chassis);




#endif