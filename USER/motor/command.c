/**
 * @file command.c
 * @author your name (you@domain.com)
 * @brief �����
 * @version 0.1
 * @date 2023-11-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

/* Includes ------------------------------------------------------------------*/
#include "command.h"

/* Private function prototypes -----------------------------------------------*/

void User_Status_Update(command_t *command);
void Cmd_Switch_Run(command_t *command);
void Cmd_Switch_Finish(command_t *command);
void Cmd_Value_Update(command_t *command);
void Cmd_HeartBeat(command_t *command);
void Cmd_Class_Update(command_t *command,bool condition);
void Cmd_Clean(command_t *command);

/* Function  body --------------------------------------------------------*/

/**
 * @brief �����ʼ��
 * 
 * @param command 
 */
void Cmd_Class_Init(command_t *command)
{
  command->user_value = false;
  command->user_value_last = false;
  command->user_status = KEEP_U;

  command->cmd_value = false;
  command->cmd_status = FINISH_C;

  command->run_time = 0;

  command->update    = Cmd_Class_Update;
  command->s_run     = Cmd_Switch_Run;
  command->s_finish  = Cmd_Switch_Finish;
  command->clean     = Cmd_Clean;
  command->heartbeat = Cmd_HeartBeat;

  command->init_flag = INIT_C;//�����ʼ�����
}
/**
 * @brief �û�����״̬����
 * 
 * @param command 
 */
void User_Status_Update(command_t *command)
{
  /* �û�����״̬���� */
  if(command->user_value == true && command->user_value_last == false)
  {
    command->user_status = SWITCH_HIGHT_U;
  }
  else if(command->user_value == false && command->user_value_last == true)
  {
    command->user_status = SWITCH_LOW_U;
  }
  else
  {
    command->user_status = KEEP_U;
  }
  /* ��ǰ����ֵΪǰһ������ֵ */
  command->user_value_last = command->user_value;
}

/**
 * @brief ����ִ��ʱ�����
 * 
 * @param command 
 */
void Cmd_HeartBeat(command_t *command)
{
  /* ����ִ��ʱ����� */
  if(command->cmd_status == RUNING_C)//��������ִ��
  {
    if (command->run_time_max != OUT_TIME_OFF)//û�йرճ�ʱ�˳�
    {
      command->run_time++;
    }
  }
  else//�����ִ��
  {
    command->run_time = 0;
  }

  /*��ʱ�˳�*/
  if(command->run_time > command->run_time_max)
  {
    Cmd_Clean(command);
  }

  /*����ֵ����*/
  Cmd_Value_Update(command);
}

/**
 * @brief ����ֵ����
 * 
 * @param command 
 */
void Cmd_Value_Update(command_t *command)
{
  /* ����ֵ���� */
  switch(command->cmd_type)
  {
    case RISE_TRIGER_C:
      if(command->user_status == SWITCH_HIGHT_U)
      {
        command->cmd_value = true;
      }
      else
      {
        command->cmd_value = false;
      }
      break;
    case FALL_TRIGER_C:
      if(command->user_status == SWITCH_LOW_U)
      {
        command->cmd_value = true;
      }
      else
      {
        command->cmd_value = false;
      }
      break;
    case HIGH_TRIGER_C:
      if (command->user_value == true)
      {
        command->cmd_value = true;
      }
      else
      {
        command->cmd_value = false;
      }
      break;
    case LOW_TRIGER_C:
      if (command->user_value == false)
      {
        command->cmd_value = true;
      }
      else
      {
        command->cmd_value = false;
      }
      break;
    default:
      break;
  }      
}

/**
 * @brief ��������� �����г�ʼ���Ż����
 * 
 * @param command 
 * @param condition �ߵ�ƽ����
 */
void Cmd_Class_Update(command_t *command,bool condition)
{
  if (command->init_flag == INIT_C)
  {
    if (condition == true)
    {
      command->user_value = true;
    }
    else
    {
      command->user_value = false;
    }
  }

  /*�û�����״̬����*/
  User_Status_Update(command);
  // /*����ֵ����*/
  // Cmd_Value_Update(command);
}

/**
 * @brief �����־λ����
 * 
 * @param command 
 */
void Cmd_Clean(command_t *command)
{
  command->user_value = false;
  command->user_value_last = false;
  command->user_status = KEEP_U;

  command->cmd_value = false;
  command->cmd_status = FINISH_C;

  command->run_time = 0;
}

/**
 * @brief �����л�Ϊ����ִ��
 * 
 * @param command 
 */
void Cmd_Switch_Run(command_t *command)
{
  command->cmd_status = RUNING_C;
}

/**
 * @brief �����л�Ϊִ�����
 * 
 * @param command 
 */
void Cmd_Switch_Finish(command_t *command)
{
  command->cmd_status = FINISH_C;
}