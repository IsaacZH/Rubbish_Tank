/**
 * @file control.c
 * @author Isaac
 * @brief 
 * @version 0.1
 * @date 2023-11-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
/* Includes ------------------------------------------------------------------*/
#include "car.h"
#include "gimbal.h"
#include "shooting.h"
#include "image.h"
#include "remote.h"
#include "led.h"
#include "rp_math.h"
#include "vision_protocol.h"
#include "communicate_protocol.h"

// #define LOB_TEST //MARK: 吊射测试模式，测试完记得关闭
/* Private function prototypes -----------------------------------------------*/

void Vision_TxData_Update();
void Car_TxData_Update(void);

void Car_Ctrl_Mode_Update(car_t *car);

void Car_Move_Mode_Update(car_t *car);
void Car_Init_Judge(car_t *car);
void RC_Move_Mode_Update(car_t *car);
void Key_Move_Mode_Update(car_t *car);

void Command_Update(car_t *car);
void Key_Command_Update(void);
void RC_Command_Update(void);

/* Private variables ---------------------------------------------------------*/

command_t command[COMMAND_LIST] = 
{
  [CAR_L90] = {
    .cmd_type = RISE_TRIGER_C,
    .run_time_max = OUT_TIME_OFF,  
		.init = Cmd_Class_Init,
	},
	[CAR_R90] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,  
	},
	[GIM_180] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = 2500,//(2.5s) 
		.init = Cmd_Class_Init, 
	},
	[GIM_UP] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init, 
	},
	[GIM_DOWM] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init, 
	},
	[GIM_RIGHT] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init, 
	},
	[GIM_LEFT] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init, 
	},
	//MARK:会在命令更新中判断是否进视觉模式改变命令类型
	[SHOOTING_FIRE] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,  
	},
	[SHOOTING_FIRING] = {
		.cmd_type = HIGH_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,
	},
	[TELESCOPE_SWITCH] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,
	},
	[AUTO_LOB] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,
	},
	[IMAGE_SWITCH_MAX] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,
	},
	[IMAGE_SWITCH_MIN] = {
		.cmd_type = RISE_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,
	},
	[CAP_ON] = {
		.cmd_type = HIGH_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,
	},
	[KILL_MYSELF] = {
		.cmd_type = HIGH_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,
	},
	[OUTPOST_VISION] = {
		.cmd_type = HIGH_TRIGER_C,
		.run_time_max = OUT_TIME_OFF,
		.init = Cmd_Class_Init,
	},

};

car_t car = 
{
  .car_reset_state = DEV_RESET_NO,
	.init_cnt_max = 3000,//(3s)
};

/* Exported functions --------------------------------------------------------*/

/**
 * @brief 命令初始化 main.c中调用一次 
 * 
 */
void Cmd_Init(void)
{
	for(uint8_t i = 0; i < COMMAND_LIST; i++)
	{
		command[i].init(&command[i]);
	}
}

/**
 * @brief 命令心跳 
 * 
 */
void Cmd_Heartbeat(void)
{
	for(uint8_t i = 0; i < COMMAND_LIST; i++)
	{
		command[i].heartbeat(&command[i]);
	}
}

/**
 * @brief 控制任务 实时任务中调用
 * 
 * @param car 
 */
void Car_Ctrl(car_t *car) 
{
	/*控制模式更新*/
	Car_Ctrl_Mode_Update(car);
	
	/* 整车模式更新 */
  Car_Move_Mode_Update(car);
	
	/* 整车命令更新 */
  Command_Update(car);
}

/**
 * @brief 通信内容更新 实时任务中调用
 * 
 * @param car 
 */
void Car_Communicate_Info_Update(car_t *car)
{
	Vision_TxData_Update();
	Car_TxData_Update();
}
/* Private functions ---------------------------------------------------------*/

/*************************外设通信********************************/

/**
 * @brief 视觉发送信息更新
 * 
 */
void Vision_TxData_Update()
{
  Vision_Tx_Info_t *tx_info = vision.tx_info;
	//模式更新
	if (command[OUTPOST_VISION].cmd_value == 1)
	{
		tx_info->mode = 2;
	}
	else
	{
  	tx_info->mode = 1;
	}
	
	//是否准备打弹更新
	if (shooting.load_state == load_OK && shooting.shooting_state == SHOOTING_RESET_OK)
	{
		tx_info->is_ready = 1;
	}
	else
	{
		tx_info->is_ready = 0;
	}
	// yaw轴角度更新
  float yaw = gimbal.base_info.yaw_imu_angle;
  tx_info->yaw =  4096.f - (yaw / 180.f) * 4096.f;

	// tx_info->yaw = tx_info->yaw + 0.1;
	// if(tx_info->yaw > 8192) tx_info->yaw = 0;

  //pitch轴角度更新
  float pitch = gimbal.base_info.pitch_imu_angle;
  tx_info->pitch =  4096.f - (pitch / 180.f) * 4096.f;

	//roll轴角度更新
	float roll = imu_sensor.info->base_info.roll;
	tx_info->roll =  4096.f - (roll / 180.f) * 4096.f;
	//pitch轴角速度更新
	tx_info->v_pitch = -imu_sensor.info->base_info.ave_rate_pitch;
	//yaw
	tx_info->v_yaw = imu_sensor.info->base_info.ave_rate_yaw;
	//offset
	tx_info->yaw_offset = gimbal.offset_info->vision_yaw_offset;
	tx_info->pitch_offset = gimbal.offset_info->vision_pitch_offset;
  //颜色更新
	if(communicate.game_robot_status_rx_info->car_color == 0)
	{
		  tx_info->my_color = 1;
	}
	else
	{
		  tx_info->my_color = 0;
	}
	// //比赛状态更新 //todo : 打训练赛关掉录视频
	if(communicate.power_heat_data_rx_info->game_status == 3 ||
		 communicate.power_heat_data_rx_info->game_status == 4)
	{
		tx_info->dune = 1;
	}
	else
	{
		tx_info->dune = 0;
	}
	// 距离计算
	
}

void Car_TxData_Update(void)
{
	/*car_data0_tx_info pack update*****************************************************/
	car_data0_tx_info_t *car_data0_tx_info = communicate.car_data0_tx_info;
	float chassis_angel = (YAW_MOTOR_ANGLE_MIDDLE - gimbal.gimbal_y->kt9025_info->state_info.encoder ) / 32768.f * 180 / 2;
	if (chassis_angel < 0)
	{
		chassis_angel += 180;
	}
	car_data0_tx_info->pitch_angel = gimbal.base_info.pitch_imu_angle * 100;
	car_data0_tx_info->target_f_speed = shooting.base_info.fri_info.target_f_speed;
	car_data0_tx_info->car_move_mode = car.car_move_mode;
	car_data0_tx_info->car_state.bit.is_find_target = vision.rx_info->is_find_target;
	car_data0_tx_info->car_state.bit.is_vision_online = vision.status->rx_state == DEV_ONLINE;
	car_data0_tx_info->car_state.bit.is_fire = command[SHOOTING_FIRE].cmd_value;
	car_data0_tx_info->car_state.bit.is_firing = command[SHOOTING_FIRING].cmd_value;
	car_data0_tx_info->car_state.bit.fri_speed_state = shooting.base_info.fri_info.fri_speed_state;
	car_data0_tx_info->car_state.bit.is_image_up = image.base_info.target_position == image.base_info.init_position;
	car_data0_tx_info->car_state.bit.is_key_ctrl = (car.car_ctrl_mode == KEY_CTRL_MODE && RC_ONLINE);
	car_data0_tx_info->lf_temp = shooting.frictionLF->rx_info.temperature;
	car_data0_tx_info->rf_temp = shooting.frictionRF->rx_info.temperature;
	
	if (car.car_ctrl_mode == KEY_CTRL_MODE)//键盘模式
	{
		if (command[CAP_ON].cmd_value == true || command[AUTO_LOB].cmd_status == RUNING_C)
		{
			car_data0_tx_info->car_state.bit.is_on_cap = 1;
		}
		else
		{
			car_data0_tx_info->car_state.bit.is_on_cap = 0;
		}
	}
	else//遥控器模式
	{
		car_data0_tx_info->car_state.bit.is_on_cap = 1;
	}
	/*car_data1_tx_info pack update******************************/
	car_data1_tx_info_t *car_data1_tx_info = communicate.car_data1_tx_info;
	car_data1_tx_info->chassis_angel = chassis_angel;
	car_data1_tx_info->vision_pitch_offset = gimbal.offset_info->vision_pitch_offset;
	car_data1_tx_info->vision_yaw_offset = gimbal.offset_info->vision_yaw_offset;
	car_data1_tx_info->B_fri_speed_diff = shooting.base_info.fri_info.fri_B_diff_speed;
	car_data1_tx_info->F_fri_speed_diff = shooting.base_info.fri_info.fri_F_diff_speed;
	car_data1_tx_info->pitch_motor_angle = gimbal.base_info.pitch_motor_angle;
	/*car_data2_tx_info pack update******************************/
	car_data2_tx_info_t *car_data2_tx_info = communicate.car_data2_tx_info;
#ifdef NEW_HERO
	car_data2_tx_info->detect_num = vision.rx_info->detect_num;
#endif
	car_data2_tx_info->ui_x = vision.rx_info->UI_x;
	car_data2_tx_info->ui_y = vision.rx_info->UI_y;
	car_data2_tx_info->lb_temp = shooting.frictionLB->rx_info.temperature;
	car_data2_tx_info->rb_temp = shooting.frictionRB->rx_info.temperature;
	/*car_data3 */
	car_data3_tx_info_t *car_data3_tx_info = communicate.car_data3_tx_info;
	car_data3_tx_info->uix_lb = vision.rx_info->uix_lb / 10;
	car_data3_tx_info->uix_rb = vision.rx_info->uix_rb / 10;
	car_data3_tx_info->uiy_lb = vision.rx_info->uiy_lb / 5;
	car_data3_tx_info->uiy_rb = vision.rx_info->uiy_rb / 5;

	car_data3_tx_info->uix_lt = vision.rx_info->uix_lt / 10;
	car_data3_tx_info->uix_rt = vision.rx_info->uix_rt / 10;
	car_data3_tx_info->uiy_lt = vision.rx_info->uiy_lt / 5;
	/*car_data4*/
	car_data4_tx_info_t *car_data4_tx_info = communicate.car_data4_tx_info;
	car_data4_tx_info->uiy_rt = vision.rx_info->uiy_rt / 5;

	car_data4_tx_info->uix_right = vision.rx_info->uix_right / 10;
	car_data4_tx_info->uiy_right = vision.rx_info->uiy_right / 5;

	car_data4_tx_info->uix_left = vision.rx_info->uix_left / 10;
	car_data4_tx_info->uiy_left = vision.rx_info->uiy_left / 5;
}

/*************************控制模式********************************/

/**
 * @brief 控制模式更新
 * 
 * @param car 
 */
void	Car_Ctrl_Mode_Update(car_t *car)
{
	//左拨杆下：键盘模式  其余都是遥控器模式
	switch(rc.base_info->s1.value)
	{
		#ifndef LOB_TEST
	case 0x02:	//左拨杆下
    car->car_ctrl_mode = KEY_CTRL_MODE;
		break;
		#endif
	default://左拨杆不是下
		car->car_ctrl_mode = RC_CTRL_MODE;
		break;
	}

//从遥控器模式转到键盘模式，进入陀螺仪模式
	switch (rc.base_info->s1.status)
	{
	case down_R://左拨杆向下拨
		car->car_move_mode = gyro_CAR;
		break;
	
	default:
		break;
	}
}






/*************************移动模式********************************/

/**
 * @brief 移动模式更新
 * 
 * @param car 
 */
void Car_Move_Mode_Update(car_t *car)
{
	Car_Init_Judge(car);
	//关控或者裁判系统断电进入offline_CAR
	if(!RC_ONLINE || (!IMAGE_MOTOR_ONLINE && !GIMBAL_MOTOR_ONLINE && !SHOOTING_MOTOR_ONLINE))
	{
		car->car_move_mode = offline_CAR;
		car->car_reset_state = DEV_RESET_NO;
		
		//清空命令
		for(uint8_t i = 0; i < COMMAND_LIST; i++)
		{
			command[i].clean(&command[i]);
		}
		car->init_cnt = 0;//清空计时

	}
	else if(car->car_reset_state == DEV_RESET_NO)//没初始化完成，进入初始化
	{
		car->car_move_mode = init_CAR;
	}
	else if (car->car_move_mode == init_CAR)
	{
		car->init_cnt = 0;//初始化成功
		car->car_move_mode = gyro_CAR;//进入默认模式（陀螺仪模式）
	}
	else 
	{
		if (car->car_ctrl_mode == RC_CTRL_MODE)//遥控器模式
		{
			RC_Move_Mode_Update(car);//右拨杆控制移动模式
		}
		else//键盘模式 
		{
			Key_Move_Mode_Update(car);
		}
	}
}

/**
 * @brief 初始化判断
 * 
 * @param car 
 */
void Car_Init_Judge(car_t *car)
{
	//判断各个设备是否初始化完成
	if(gimbal.gimbal_reset_state == DEV_RESET_OK &&
		 image.image_reset_state == DEV_RESET_OK )
	{
		car->car_reset_state = DEV_RESET_OK;
	}
 //初始化计时
	if (car->car_move_mode == init_CAR)
	{
		car->init_cnt ++;
	}
	//初始化超时退出
	if (car->init_cnt >= car->init_cnt_max)
	{
		car->car_reset_state = DEV_RESET_OK;
		car->init_cnt = car->init_cnt_max;
	}
}
/**
 * @brief 遥控模式整车移动模式更新
 * 
 * @param car 
 */
void RC_Move_Mode_Update(car_t *car)
{
	static uint8_t thumbweheel_status,thumbweheel_last_status;
	thumbweheel_last_status = thumbweheel_status;
	thumbweheel_status = rc.base_info->thumbwheel.status;

	switch(rc.base_info->s2.value)
	{
	case 0x01:  //右拨杆上
		#ifdef LOB_TEST
		car->car_move_mode = lob_CAR ;
		#else
		car->car_move_mode = mec_CAR ;
		#endif
		break;
	case 0x03:	//右拨杆中
		//拨轮向上切换陀螺仪和小陀螺
		#ifdef LOB_TEST
		car->car_move_mode = mec_CAR ;
		#else
		if(thumbweheel_status != thumbweheel_last_status)
		{
			if(thumbweheel_status == down_R)
			{
				if(car->car_move_mode == gyro_CAR)
				{
					car->car_move_mode = cycle_CAR;
				}
				else if(car->car_move_mode == cycle_CAR)
				{
					car->car_move_mode = gyro_CAR;
				}
			}
		}
		//拨杆中间时，切换到陀螺仪模式
		if (car->car_move_mode != gyro_CAR &&
				car->car_move_mode != cycle_CAR)
		{
			car->car_move_mode = gyro_CAR;
		}
		#endif
		break;
	case 0x02:	//右拨杆下
		//拨轮跳变切换到小陀螺
		if(thumbweheel_status != thumbweheel_last_status)
		{
			if(thumbweheel_status == down_R)
			{
				if(car->car_move_mode == vision_gyro_CAR)
				{
					car->car_move_mode = vision_cycle_CAR;
				}
				else if(car->car_move_mode == vision_cycle_CAR)
				{
					car->car_move_mode = vision_gyro_CAR;
				}
			}
		}
		//如果不是视觉模式，切换到视觉陀螺仪模式
		if (car->car_move_mode != vision_gyro_CAR &&
				car->car_move_mode != vision_cycle_CAR)
		{  
			car->car_move_mode = vision_gyro_CAR;
		}
		break;
	default:
		break;			
	}
}

/**
 * @brief 键盘模式整车移动模式更新 //todo: lob_CAR have not changed
 * 
 * @param car 
 */
void Key_Move_Mode_Update(car_t *car)
{
	//auto_lob命令进入机械模式
	if (command[AUTO_LOB].cmd_value == true)
	{
		car->car_move_mode = mec_CAR;
	}

	switch (car->car_move_mode)
	{
	case mec_CAR:
		//右键按下，进入视觉陀螺仪模式
		if(rc.base_info->mouse_btn_r.value == 1)
		{
			car->car_move_mode = vision_gyro_CAR;
		}
		//F键按下，进入小陀螺模式
		if(rc.base_info->F.status == down_K)
		{
			car->car_move_mode = cycle_CAR;
		}
		//Ctrl一直按下，进入吊射模式
		if (rc.base_info->Ctrl.value == 1)
		{
			car->car_move_mode = lob_CAR;
		}
		//Shift键按下，进入陀螺仪模式
		if(rc.base_info->Shift.status == down_K) 
		{
			car->car_move_mode = gyro_CAR;
		}
		break;
	case lob_CAR:
		//Ctrl没一直按下，进入机械模式
		if (rc.base_info->Ctrl.value != 1)
		{
			car->car_move_mode = mec_CAR;
		}
		//Shift键按下，进入陀螺仪模式
		if(rc.base_info->Shift.status == down_K) 
		{
			car->car_move_mode = gyro_CAR;
		}
		break;
	case gyro_CAR:
		//右键按下，进入视觉陀螺仪模式
		if(rc.base_info->mouse_btn_r.value == 1)
		{
			car->car_move_mode = vision_gyro_CAR;
		}
		//C键按下，进入机械模式
		if(rc.base_info->C.status == down_K)
		{
			car->car_move_mode = mec_CAR;
		}
		//F键按下，进入小陀螺模式
		if(rc.base_info->F.status == down_K)
		{
			car->car_move_mode = cycle_CAR;
		}
		break;

	case cycle_CAR:
		//右键按下，进入视觉小陀螺模式
		if(rc.base_info->mouse_btn_r.value == 1)
		{
			car->car_move_mode = vision_cycle_CAR;
		}
		//C键按下，进入机械模式
		if(rc.base_info->C.status == down_K)
		{
			car->car_move_mode = mec_CAR;
		}
		//F键按下，进入陀螺仪模式
		if(rc.base_info->F.status == down_K)
		{
			car->car_move_mode = gyro_CAR;
		}
		break;

	case vision_gyro_CAR:
		//右键松开，进入陀螺仪模式
		if(rc.base_info->mouse_btn_r.value == 0 )
		{
			car->car_move_mode = gyro_CAR;
		}
		//F键按下，进入视觉小陀螺模式
		if(rc.base_info->F.status == down_K)
		{
			car->car_move_mode = vision_cycle_CAR;
		}
		break;

	case vision_cycle_CAR:
		//右键松开，进入小陀螺模式
		if(rc.base_info->mouse_btn_r.value == 0)
		{
			car->car_move_mode = cycle_CAR;
		}
		//F键按下，进入视觉陀螺仪模式
		if(rc.base_info->F.status == down_K)
		{
			car->car_move_mode = vision_gyro_CAR;
		}
		break;

	default:
		break;
	}

	if (command[OUTPOST_VISION].cmd_value == 1)
	{
		if (car->car_move_mode == cycle_CAR)
		{
			car->car_move_mode = vision_cycle_CAR;
		}
		else
		{
			car->car_move_mode = vision_gyro_CAR;
		}
		
	}
	
}






/***************************命令********************************/

/**
 * @brief 命令更新 car_ctrl中调用
 * 
 * @param car 
 */
void Command_Update(car_t *car)
{
  if(car->car_ctrl_mode == RC_CTRL_MODE)
  {
    RC_Command_Update();
  }
  else if(car->car_ctrl_mode == KEY_CTRL_MODE)
  {
    Key_Command_Update();
  }
}

/**
 * @brief 键盘模式命令更新
 * 
 */
void Key_Command_Update(void)
{
		//视觉模式下自动打弹
	if(car.car_move_mode == vision_cycle_CAR ||
		 car.car_move_mode == vision_gyro_CAR)
	{
		command[SHOOTING_FIRE].cmd_type = HIGH_TRIGER_C;
		command[SHOOTING_FIRE].update(&command[SHOOTING_FIRE],
																  rc.base_info->mouse_btn_l.status == short_press_K ||
																  rc.base_info->mouse_btn_l.status == long_press_K ||
																	rc.base_info->Z.value == 1);

	}
	else
	{
		command[SHOOTING_FIRE].cmd_type = RISE_TRIGER_C;
		//非视觉模式下才检测连发
		command[SHOOTING_FIRING].update(&command[SHOOTING_FIRING],
																		rc.base_info->mouse_btn_l.status == long_press_K);	
		command[SHOOTING_FIRE].update(&command[SHOOTING_FIRE],
																		rc.base_info->mouse_btn_l.status == short_press_K);

	}

  command[CAR_L90].update(&command[CAR_L90],
                                    rc.base_info->Q.status == down_K);
	command[CAR_R90].update(&command[CAR_R90],
																		rc.base_info->E.status == down_K);
	command[GIM_180].update(&command[GIM_180],
																		rc.base_info->R.status == down_K);
	//ctrl命令
	command[GIM_UP].update(&command[GIM_UP],
																		rc.base_info->Ctrl.value != 0 && rc.base_info->W.status == down_K);
	command[GIM_DOWM].update(&command[GIM_DOWM],
																		rc.base_info->Ctrl.value != 0 && rc.base_info->S.status == down_K);
	command[GIM_LEFT].update(&command[GIM_LEFT],
																		rc.base_info->Ctrl.value != 0 && rc.base_info->A.status == down_K);
	command[GIM_RIGHT].update(&command[GIM_RIGHT],
																		rc.base_info->Ctrl.value != 0 && rc.base_info->D.status == down_K);
		//MARK: 会在Key_Move_Mode_Update中判断进入机械模式
	command[AUTO_LOB].update(&command[AUTO_LOB],
																		rc.base_info->Ctrl.value != 0 && rc.base_info->V.status == down_K);
	
	//图传命令
	command[TELESCOPE_SWITCH].update(&command[TELESCOPE_SWITCH],
																	  rc.base_info->V.status == down_K && rc.base_info->Ctrl.value == 0);
	command[IMAGE_SWITCH_MAX].update(&command[IMAGE_SWITCH_MAX],
																		rc.base_info->mouse_vz > 8);//max:10
  command[IMAGE_SWITCH_MIN].update(&command[IMAGE_SWITCH_MIN],
																		rc.base_info->mouse_vz < -8);													
	
	//超电命令
	command[CAP_ON].update(&command[CAP_ON],rc.base_info->Shift.value == 1);

	//自爆模式
	command[KILL_MYSELF].update(&command[KILL_MYSELF],rc.base_info->G.value == 1);

	//视觉前哨
	command[OUTPOST_VISION].update(&command[OUTPOST_VISION],rc.base_info->Z.value == 1);
}

/**
 * @brief 遥控器模式命令更新
 * 
 */
void RC_Command_Update(void)
{
	//视觉模式下自动打弹
	if(car.car_move_mode == vision_cycle_CAR ||
		 car.car_move_mode == vision_gyro_CAR)
	{
		command[SHOOTING_FIRE].cmd_type = HIGH_TRIGER_C;
	}
	else
	{
		command[SHOOTING_FIRE].cmd_type = RISE_TRIGER_C;
	}

	command[SHOOTING_FIRE].update(&command[SHOOTING_FIRE],
																		rc.base_info->s1.value == 0x01);																

	command[GIM_180].update(&command[GIM_180],
																		rc.base_info->thumbwheel.status == up_R);

	command[TELESCOPE_SWITCH].update(&command[TELESCOPE_SWITCH],
																		rc.base_info->thumbwheel.status == up_R ||
																		rc.base_info->thumbwheel.status == down_R);
}

