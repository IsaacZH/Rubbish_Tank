/**
  ******************************************************************************
  * @file           : RP_drv
  * @brief          : 
  * @note           : start 2022-12-12 
  ******************************************************************************
  */

#include "config_can.h"
#include "config.h"
#include "device.h"
#include "communicate_protocol.h"

/**
 * @brief can2收
 * 
 * @param canId CAN ID
 * @param rxBuf 数据帧
 */
void CAN2_RX(uint32_t canId, uint8_t *rxBuf)
{
	switch (canId)
	{
		case ID_GIMBAL_PITCH:
			gimbal_motor[GIMBAL_PITCH].rx(&gimbal_motor[GIMBAL_PITCH],rxBuf,8);
			break;
		case ID_LIMIT:
			shooting_motor[LIMIT].rx(&shooting_motor[LIMIT],rxBuf,8);
			break;
		case ID_IMAGE:
			image_motor.rx(&image_motor,rxBuf,8);
			break;
		case ID_FRICTION_LF:
			shooting_motor[FRICTION_LF].rx(&shooting_motor[FRICTION_LF],rxBuf,8);
			break;
		case ID_FRICTION_RF:
			shooting_motor[FRICTION_RF].rx(&shooting_motor[FRICTION_RF],rxBuf,8);
			break;
		case ID_FRICTION_LB:
			shooting_motor[FRICTION_LB].rx(&shooting_motor[FRICTION_LB],rxBuf,8);
			break;
		case ID_FRICTION_RB:
			shooting_motor[FRICTION_RB].rx(&shooting_motor[FRICTION_RB],rxBuf,8);
			break;
		default:
			break;
	}
		 
}

/**
  * @Name    CAN1_RX
  * @brief   can1收
  * @param   canId：canid  
	* @param   rxBuf：数据帧
  * @retval 
  * @author  
  * @Date    
**/
void CAN1_RX(uint32_t canId, uint8_t *rxBuf)
{
	switch (canId)
	{	
		case ID_GIMBAL_YAW:
			gimbal_motor[GIMBAL_YAW].rx(&gimbal_motor[GIMBAL_YAW],rxBuf,8);
			break;
		case ID_DAIL:
			shooting_motor[DAIL].rx(&shooting_motor[DAIL],rxBuf,8);
			break;
		case GAME_ROBOT_STATUS_RX_ID:
			Game_Robot_Status_Rx(rxBuf);
			break;
		case POWER_HEAT_DATA_RX_ID:
			Power_Heat_Data_Rx(rxBuf);
			break;
		case SHOOT_DATA_RX_ID:
			Shoot_Data_Rx(rxBuf);
			break;	
		default:
			break;
	}
}




