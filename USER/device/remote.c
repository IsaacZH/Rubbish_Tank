#include "remote.h"
#include "string.h"
#include "stdbool.h"
#include "config.h"

bool rc_interrupt = false;                  //ң���������ж�

rc_base_info_t rc_base_info;
rc_info_t rc_info;
rc_t rc = 
{
  .base_info = &rc_base_info,
  .info = &rc_info,
};

/**
  * @brief  ң������ʼ��
  */
void RC_Init(rc_t *rc)
{
	/* ������Ϣ��ʼ�� */
  rc_base_info_init(rc->base_info);
	/* ��Ϣ��ʼ�� */
	rc_info_init(rc->info);
  /* �жϱ�־��� */
  rc_interrupt_init(rc->base_info);
}

/**
  * @brief  ������Ϣ��ʼ��
  */
void rc_base_info_init(rc_base_info_t *info)
{
	/* ������Ϣ���� */
  memset(info,0,sizeof(rc_base_info_t));
	/* ������ť״̬��ʼ�� */
  info->s1.status = keep_R;
  info->s2.status = keep_R;
}


/**
  * @brief  ��Ϣ��ʼ��
  */
void rc_info_init(rc_info_t *info)
{
	/* ����ʧ������ */
  info->offline_cnt = REMOTE_OFFLINE_CNT_MAX;
  /* ����״̬ */
  info->status = DEV_OFFLINE;
}

/**
  * @brief  �жϱ�־���
  */
void rc_interrupt_init(rc_base_info_t *info)
{
  rc_interrupt = false;                  //ң���������ж�
}



/**
  * @brief  ң�����δ�����
  */
void rc_heart_beat(rc_t *rc)
{
  rc->info->offline_cnt ++;
  if(rc->info->offline_cnt > REMOTE_OFFLINE_CNT_MAX)
    RC_Init(rc);
	else 
		remote_soft_reset_check(rc);//��λ
  
}
/**
  * @brief  ң����Ϣ����
  */
void rc_base_info_update(rc_t *rc, uint8_t *rxBuf)
{
	rc_base_info_t *info = rc->base_info;
  info->ch0 = ((rxBuf[1]|rxBuf[2]<< 8) & 0x07FF);
  info->ch0 -= 1000;
  info->ch1 = ((rxBuf[2]>>3|rxBuf[3]<<5) & 0x07FF);
  info->ch1 -= 1000;
  info->ch2 = ((rxBuf[3]>>6|rxBuf[4]<<2|rxBuf[5]<<10) & 0x07FF);
  info->ch2 -= 1000;
  info->ch3 = ((rxBuf[5]>>1|rxBuf[6]<<7) & 0x07FF);
  info->ch3 -= 1000;
  info->s1.value = ((rxBuf[6]>>4|rxBuf[7]<<4) & 0x07FF)%500;
  info->s2.value = ((rxBuf[7]>>7|rxBuf[8]<<1|rxBuf[9]<<9) & 0x07FF)%500;
		
	/* �жϸ��� */
	if(rxBuf[23]!=16)
	{
		rc_interrupt = true;
		rc->info->offline_cnt = 0;
		rc->info->status = DEV_ONLINE;
	}
}

/**
  * @brief  ��������
  */
void rc_ctrl(rc_t *rc)
{
  static uint8_t last_rc_status = 0;
  if(rc_interrupt == true)
  {
    rc_interrupt = false;
    if(last_rc_status == rc->info->status)//ң����״̬û��
    {
      /* ң��������״̬���� */
      rc_switch_status_interrupt_update(rc->base_info);
    }
    else//����
    {
      rc->base_info->s1.value_last = rc->base_info->s1.value;
      rc->base_info->s2.value_last = rc->base_info->s2.value;
    }

  }

  /*������һ��ң����״̬*/
  last_rc_status = rc->info->status;
}

/**
  * @brief  ң��������״̬�����жϲ�����
  */
void rc_switch_status_interrupt_update(rc_base_info_t *info)
{
  /* �󲦸��ж� */
  if(info->s1.value != info->s1.value_last)
  {
    switch(info->s1.value)
    {
      case 1:
        info->s1.status = up_R;
        break;
      case 2:
        info->s1.status = mid_R;
        break;
      case 3:
        info->s1.status = down_R;
        break;
      default:
        break;
    }
    info->s1.value_last = info->s1.value;
  }
  else 
    info->s1.status = keep_R;
  
  /* �Ҳ����ж� */
  if(info->s2.value != info->s2.value_last)
  {
    switch(info->s2.value)
    {
      case 1:
        info->s2.status = up_R;
        break;
      case 2:
        info->s2.status = mid_R;
        break;
      case 3:
        info->s2.status = down_R;
        break;
      default:
        break;
    }
    info->s2.value_last = info->s2.value;
  }
  else 
    info->s2.status = keep_R;
  
}



void remote_soft_reset_check(rc_t *rc)
{

    // if((rc->base_info->s1.value == 0x02)&&(rc->base_info->s2.value == 0x02))
    //   if(rc->base_info->thumbwheel.status == up_R || rc->base_info->thumbwheel.status == down_R)
    //     HAL_NVIC_SystemReset();

}
