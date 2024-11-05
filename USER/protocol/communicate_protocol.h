/**
 * @file communicate_protocol.h
 * @author Isaac
 * @brief ���ͨ��Э�飨�����أ�
 * @version 0.1
 * @date 2023-11-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __COMMUNICATE_PROTOCOL_H
#define __COMMUNICATE_PROTOCOL_H

#include "config.h"

#define POWER_HEAT_DATA_RX_ID        (0x100)//power_heat_data����ID
#define GAME_ROBOT_STATUS_RX_ID      (0x101)//game_robot_status����ID
#define SHOOT_DATA_RX_ID             (0x102)//shoot_data����ID_
#define CHASSIS_DATA_TX_ID           (0X250)
#define CAR_DATA_TX_ID               (0X104)
#define COMMUNICATE_OFFLINE_CNT_MAX  (40)//����������(ms)

/**
 * @brief ������״̬���սṹ��
 * 
 */
typedef struct __packed
{
	uint16_t shooter_cooling_limit;			//������ 42mm ǹ����������
	uint8_t car_color;                  //��ɫ:1  ��ɫ:0
	uint16_t chassis_power_limit;       //���̹�������
	uint8_t is_laser_on;								//ͨ��ǰ��վѪ���ж��Ƿ񿪼���											
	uint16_t base_distance;             //ǹ��
}game_robot_status_rx_info_t;

/**
 * @brief ���̹��ʺ�ǹ���������սṹ��
 * 
 */
typedef struct __packed
{
  int16_t chassis_power_buffer;           //���̻��湦��
	uint16_t shooter_cooling_heat; 					//������ 42mm ǹ������
	uint8_t rfid;                           //�ߵ�RFID״̬ 1��ˢ�� 0��ûˢ��
	uint16_t remain_HP;                     //ʣ��Ѫ��
	uint8_t game_status;                    //����״̬
}power_heat_data_rx_info_t;

/**
 * @brief ���ٽ��սṹ��
 * 
 */
typedef struct __packed
{
	float shooting_speed;                   //����
	uint32_t keep_null;											//����λ
}shoot_data_rx_info_t;


/**
 * @brief ���̰�
 * 
 */
typedef struct __packed
{
	int16_t target_front_speed;             //����Ŀ��ǰ���ٶ�
	int16_t target_right_speed;							//����Ŀ��ƽ���ٶ�
	int16_t target_cycle_speed;							//����Ŀ����ת�ٶ�

	uint16_t max_speed : 15;								//����ٶ�����
	uint16_t pid_mode : 1;									//0:speed  1:position 
	
}chassis_data_tx_info_t;

/**
 * @brief ������Ϣ��
 * 
 */
typedef struct __packed
{
	uint8_t pack_id;//��ID

	uint16_t car_move_mode : 3;//�ƶ�ģʽ
	uint16_t target_f_speed : 13 ;//Ħ����Ŀ���ٶ�
	int16_t pitch_angel;//������ *100
	union 
	{
		uint8_t state;  //����״̬
		struct
		{
			uint8_t is_find_target : 1;//�Ӿ��Ƿ��ҵ�Ŀ��
			uint8_t is_vision_online : 1;//�Ӿ��Ƿ�����
			uint8_t is_on_cap : 1;//�Ƿ񿪳���
			uint8_t is_fire : 1;//�Ƿ񵥷�
			uint8_t is_firing : 1;//�Ƿ�����
			uint8_t fri_speed_state : 1;//Ħ����ת���Ƿ�����
			uint8_t is_image_up : 1;//ͼ���Ƿ���
			uint8_t is_key_ctrl : 1;//ͷ�Ƿ�ǰ
		}bit;
	}car_state;

	uint8_t lf_temp;
	uint8_t rf_temp;
}car_data0_tx_info_t;

/**
 * @brief �Ӿ�ƫ�ð�
 * 
 */
typedef struct __packed
{
	uint8_t pack_id;//ID 1

	uint8_t chassis_angel;//������ԽǶ�

	int8_t vision_yaw_offset;
	int8_t vision_pitch_offset;

  uint8_t F_fri_speed_diff;
	uint8_t B_fri_speed_diff;

	int16_t pitch_motor_angle;
}car_data1_tx_info_t;

/**
 * @brief 2��
 * 
 */
typedef struct __packed
{
	uint8_t pack_id;

	uint16_t ui_x;
	uint16_t ui_y;
	uint8_t  detect_num;

	uint8_t lb_temp;
	uint8_t rb_temp;
}car_data2_tx_info_t;

/**
 * @brief 3��
 * 
 */
typedef struct __packed
{
	uint8_t pack_id;

	uint8_t uix_lt;
	uint8_t uiy_lt;

	uint8_t uix_lb;
	uint8_t uiy_lb;

	uint8_t uix_rb;
	uint8_t uiy_rb;

	uint8_t uix_rt;

}car_data3_tx_info_t;

/**
 * @brief 4��
 * 
 */
typedef struct __packed
{
	uint8_t pack_id;

	uint8_t uiy_rt;

	uint8_t uix_left;
	uint8_t uiy_left;

	uint8_t uix_right;
	uint8_t uiy_right;

	uint16_t keep_null;
}car_data4_tx_info_t;

/**
 * @brief ���ͨ��״̬�ṹ��
 * 
 */
typedef struct __packed
{
	Dev_Work_State_e work_state;						//����״̬
	uint8_t offline_cnt;										//���߼���
	uint8_t offline_cnt_max;								//����������
	float game_robot_status_fre;
	float power_heat_data_fre;
	float shoot_data_fre;
}communicate_status_t;

/**
 * @brief ���ͨ���ܽṹ��
 * 
 */
typedef struct __packed
{
	game_robot_status_rx_info_t *game_robot_status_rx_info;
	power_heat_data_rx_info_t *power_heat_data_rx_info;
	shoot_data_rx_info_t *shoot_data_rx_info;
	chassis_data_tx_info_t *chassis_data_tx_info;
	car_data0_tx_info_t *car_data0_tx_info;
	car_data1_tx_info_t *car_data1_tx_info;
	car_data2_tx_info_t *car_data2_tx_info;
	car_data3_tx_info_t *car_data3_tx_info;
	car_data4_tx_info_t *car_data4_tx_info;
	communicate_status_t *status;
}communicate_t;


extern communicate_t communicate;


void Game_Robot_Status_Rx(uint8_t *rxBuf);
void Power_Heat_Data_Rx(uint8_t *rxBuf);
void Shoot_Data_Rx(uint8_t *rxBuf);
void Chassis_Data_Tx(void);
void Communicate_Heartbeat(void);
void Car_Data_Tx(void);
#endif
