// #ifndef __CAR_H
// #define __CAR_H

// #include "command.h"
// #include "config.h"

// #include "main.h"
// /**
//  * @brief �����б�
//  * 
//  */
// enum 
// {
//   CAR_L90,//��ת90��
//   CAR_R90,//��ת90��
//   GIM_180,//��̨ת180��
//   GIM_UP,//��̨����΢��
//   GIM_DOWM,//��̨����΢��
//   GIM_RIGHT,//��̨����΢��
//   GIM_LEFT,//��̨����΢��

//   SHOOTING_FIRE,//����
//   SHOOTING_FIRING,//����
  
//   TELESCOPE_SWITCH,//�л�����״̬

//   AUTO_LOB,//һ������

//   IMAGE_SWITCH_MAX,//ͼ��������
//   IMAGE_SWITCH_MIN,//ͼ��������

//   CAP_ON,//�򿪳���

//   KILL_MYSELF,//�Ա�

//   OUTPOST_VISION,//�Ӿ�ǰ��ģʽ
  
//   COMMAND_LIST,
// };

// /* ���ж�ģʽö�� */
// typedef enum 
// {
//   offline_CAR,        //����ģʽ         0
//   init_CAR,           //��ʼ��ģʽ       1
//   mec_CAR,            //��еģʽ         2
//   gyro_CAR,           //������ģʽ       3
// 	cycle_CAR,					//С����ģʽ       4
//   vision_gyro_CAR,    //�Ӿ�������ģʽ   5
//   vision_cycle_CAR,   //�Ӿ�С����ģʽ   6
//   lob_CAR,            //����ģʽ(����λ�û�)         7

// }Car_Move_Mode_e;

// /* ������ģʽö�� */
// typedef enum 
// {
//   RC_CTRL_MODE,        //ң����ģʽ  0
//   KEY_CTRL_MODE,       //����ģʽ    1

// }Car_Ctrl_Mode_e;

// /**
//  * @brief car��ṹ��
//  * 
//  */
// typedef struct __packed
// {
//   Car_Move_Mode_e car_move_mode;
// 	Dev_Reset_State_e	 car_reset_state;
// 	Car_Ctrl_Mode_e  car_ctrl_mode;
  
//   uint16_t init_cnt;
//   uint16_t init_cnt_max;
// }car_t;

// extern car_t car;
// extern command_t command[COMMAND_LIST];

// void Car_Ctrl(car_t *car);
// void Car_Communicate_Info_Update(car_t *car);

// void Cmd_Heartbeat(void);
// void Cmd_Init(void);


// #endif