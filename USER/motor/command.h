#ifndef __COMMAND_H
#define __COMMAND_H

#include "stdbool.h"
#include "stm32f4xx_hal.h"

#define OUT_TIME_OFF  (0xFFFF)//�رճ�ʱ�˳�

/**
 * @brief ��������ö��
 */
typedef enum
{
  HIGH_TRIGER_C, //�ߵ�ƽ����
  LOW_TRIGER_C,  //�͵�ƽ����
  RISE_TRIGER_C, //�����ش���
  FALL_TRIGER_C, //�½��ش���
}Cmd_Type_e;

/**
 * @brief �û�����״̬ö��
 */
typedef enum 
{
  KEEP_U,         // ����
  SWITCH_HIGHT_U, // ����(������)
  SWITCH_LOW_U,   // �ɿ�(�½���)
}User_Status_e;

/**
 * @brief ����״̬ö��
 */
typedef enum 
{
	FINISH_C, //ִ�����
	RUNING_C, //ִ����
}Cmd_Status_e;

/**
 * @brief �����ʼ��״̬ö��
 * 
 */
typedef enum
{
  DEINIT_C, //δ��ʼ��
  INIT_C,   //��ʼ��
}Cmd_Init_e;

/**
 * @brief ����ṹ��
 */
typedef struct command_class_t
{
  bool cmd_value;//����ֵ�����ϴ���������True��
  Cmd_Status_e cmd_status;//����״̬

  Cmd_Type_e cmd_type;//��������
  
  bool user_value;//��ǰ�û�����
  bool user_value_last;//��һ���û�����
  User_Status_e user_status;//�û�����״̬

  uint16_t run_time;//����ִ��ʱ��
  uint16_t run_time_max;//����ִ��ʱ������

  Cmd_Init_e init_flag;//�����ʼ��״̬

  void (*init)(struct command_class_t *command);//�����ʼ��
  void (*heartbeat)(struct command_class_t *command);//��������
  void (*update)(struct command_class_t *command,bool condition);//��ȡ�û�����
  void (*clean)(struct command_class_t *command);//�������
  void (*s_run)(struct command_class_t *command);//�����л�������ִ��
  void (*s_finish)(struct command_class_t *command);//�����л���ִ�����
}command_t;


void Cmd_Class_Init(command_t *commond);
#endif