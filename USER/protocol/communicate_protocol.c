/**
 * @file communicate_protocol.c
 * @author Isaac
 * @brief ���ͨ��Э�飨�����أ�
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "communicate_protocol.h"
#include "drv_can.h"
#include "vision_protocol.h"
#include "stdbool.h"
#include "string.h"
#include "remote.h"
#include "shooting.h"
 
game_robot_status_rx_info_t game_robot_status_rx_info;
power_heat_data_rx_info_t power_heat_data_rx_info;
shoot_data_rx_info_t shoot_data_rx_info;
chassis_data_tx_info_t chassis_data_tx_info;
car_data0_tx_info_t car_data0_tx_info = 
{
  .pack_id = 0,
};
car_data1_tx_info_t car_data1_tx_info =
{
  .pack_id = 1,
};
car_data2_tx_info_t car_data2_tx_info = 
{
  .pack_id = 2,
};
car_data3_tx_info_t car_data3_tx_info = 
{
  .pack_id = 3,
};
car_data4_tx_info_t car_data4_tx_info =
{
  .pack_id = 4,
};

communicate_status_t status =
{
  .work_state = DEV_OFFLINE,
  .offline_cnt = 0,
  .offline_cnt_max = COMMUNICATE_OFFLINE_CNT_MAX,
};
communicate_t communicate =
{
  .game_robot_status_rx_info = &game_robot_status_rx_info,
  .power_heat_data_rx_info = &power_heat_data_rx_info,
  .shoot_data_rx_info = &shoot_data_rx_info,
  .chassis_data_tx_info = &chassis_data_tx_info,
  .car_data0_tx_info = &car_data0_tx_info,
  .car_data1_tx_info = &car_data1_tx_info,
  .car_data2_tx_info = &car_data2_tx_info,
  .car_data3_tx_info = &car_data3_tx_info,
  .car_data4_tx_info = &car_data4_tx_info,
  .status = &status,
};

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

/**
 * @brief Game_Robot_Status���ݽ��ܴ���
 * 
 * @param rxBuf 
 */
void Game_Robot_Status_Rx(uint8_t *rxBuf)
{
  memcpy(&game_robot_status_rx_info, rxBuf, sizeof(game_robot_status_rx_info_t));
  communicate.status->offline_cnt = 0;

  static uint32_t last_rx_tick = 0;
  communicate.status->game_robot_status_fre = HAL_GetTick()-last_rx_tick;
  last_rx_tick = HAL_GetTick();
}

/**
 * @brief Power_Heat_Data���ݽ��ܴ���
 * 
 * @param rxBuf 
 */
void Power_Heat_Data_Rx(uint8_t *rxBuf)
{
  memcpy(&power_heat_data_rx_info, rxBuf, sizeof(power_heat_data_rx_info_t));
  communicate.status->offline_cnt = 0;

  static uint32_t last_rx_tick = 0;
  communicate.status->power_heat_data_fre = HAL_GetTick()-last_rx_tick;
  last_rx_tick = HAL_GetTick();
}

uint32_t shoot_cnt = 0;
/**
 * @brief Shoot_Data���ݽ��ܴ���
 * 
 * @param rxBuf 
 */
void Shoot_Data_Rx(uint8_t *rxBuf)
{
  memcpy(&shoot_data_rx_info, rxBuf, sizeof(shoot_data_rx_info_t));
  communicate.status->offline_cnt = 0;
  Vison_Interrupt_Update();//�Ӿ��жϸ���
  Shooting_Cmd_Excute_Tick_Calculating(1);//�������ʱ�����
  Shooting_Fri_Speed_Adapt(&shooting);//��������Ӧ

  
  static uint32_t last_rx_tick = 0;
  communicate.status->shoot_data_fre = HAL_GetTick()-last_rx_tick;
  last_rx_tick = HAL_GetTick();
	
	shoot_cnt++;//�򵯼���
}

/**
 * @brief Chassis_Data���ݷ���
 * 
 * @note ��chassis_work�е��ã����زŻᷢ 
 */
void Chassis_Data_Tx(void)
{
  uint8_t tx_buff[8];
  uint32_t txMailBox;
  if(RC_ONLINE)
  {
    memcpy(tx_buff, &chassis_data_tx_info, sizeof(chassis_data_tx_info_t));
    HAL_CAN_TxHeadeInit(CHASSIS_DATA_TX_ID);//����ID
    HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,tx_buff,(uint32_t *)CAN_TX_MAILBOX0);
  }
}

void Car_Data_Tx(void)
{
  static uint32_t last_tx_tick = 0;
  static uint8_t tx_pack_flag = 0;
  uint8_t tx_buff[8];
  uint32_t txMailBox;
  HAL_CAN_TxHeadeInit(CAR_DATA_TX_ID);//����ID
  
  if(HAL_GetTick()-last_tx_tick > 5)//ÿ5ms��һ��
  {
    /*���÷��Ͱ�*/
      if (tx_pack_flag == 0)//����
    {
      memcpy(tx_buff, &car_data0_tx_info,sizeof(car_data0_tx_info_t));
      HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,tx_buff,&txMailBox);
    }
    else if (tx_pack_flag == 1)
    {
      memcpy(tx_buff, &car_data1_tx_info,sizeof(car_data1_tx_info_t));
      HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,tx_buff,&txMailBox);
    }
    else if (tx_pack_flag == 2)
    {
      memcpy(tx_buff, &car_data2_tx_info,sizeof(car_data2_tx_info_t));
      HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,tx_buff,&txMailBox);
    }
    else if (tx_pack_flag == 3)
    {
      memcpy(tx_buff, &car_data3_tx_info,sizeof(car_data3_tx_info_t));
      HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,tx_buff,&txMailBox);
    }
    else if (tx_pack_flag == 4)
    {
      memcpy(tx_buff, &car_data4_tx_info,sizeof(car_data4_tx_info_t));
      HAL_CAN_AddTxMessage(&hcan1,&CAN_TxHeadeType,tx_buff,&txMailBox);
    }
    tx_pack_flag ++;
    if (tx_pack_flag > 4) tx_pack_flag = 0;

    last_tx_tick = HAL_GetTick(); 
  }
}
  

/**
 * @brief ͨ������ ��������е���
 * 
 */
void Communicate_Heartbeat(void)
{
	communicate.status->offline_cnt++;
	if(communicate.status->offline_cnt > \
     communicate.status->offline_cnt_max)
  {
    communicate.status->offline_cnt = \
    communicate.status->offline_cnt_max;
    communicate.status->work_state = DEV_OFFLINE;
  }
  else if(communicate.status->work_state == DEV_OFFLINE)
  {
    communicate.status->work_state = DEV_ONLINE;
  }
  
}