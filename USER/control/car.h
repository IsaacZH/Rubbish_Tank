// #ifndef __CAR_H
// #define __CAR_H

// #include "command.h"
// #include "config.h"

// #include "main.h"
// /**
//  * @brief 命令列表
//  * 
//  */
// enum 
// {
//   CAR_L90,//左转90度
//   CAR_R90,//右转90度
//   GIM_180,//云台转180度
//   GIM_UP,//云台向上微调
//   GIM_DOWM,//云台向下微调
//   GIM_RIGHT,//云台向右微调
//   GIM_LEFT,//云台向左微调

//   SHOOTING_FIRE,//单发
//   SHOOTING_FIRING,//连发
  
//   TELESCOPE_SWITCH,//切换倍镜状态

//   AUTO_LOB,//一键吊射

//   IMAGE_SWITCH_MAX,//图传到最上
//   IMAGE_SWITCH_MIN,//图传到最下

//   CAP_ON,//打开超电

//   KILL_MYSELF,//自爆

//   OUTPOST_VISION,//视觉前哨模式
  
//   COMMAND_LIST,
// };

// /* 车行动模式枚举 */
// typedef enum 
// {
//   offline_CAR,        //离线模式         0
//   init_CAR,           //初始化模式       1
//   mec_CAR,            //机械模式         2
//   gyro_CAR,           //陀螺仪模式       3
// 	cycle_CAR,					//小陀螺模式       4
//   vision_gyro_CAR,    //视觉陀螺仪模式   5
//   vision_cycle_CAR,   //视觉小陀螺模式   6
//   lob_CAR,            //抛球模式(底盘位置环)         7

// }Car_Move_Mode_e;

// /* 车控制模式枚举 */
// typedef enum 
// {
//   RC_CTRL_MODE,        //遥控器模式  0
//   KEY_CTRL_MODE,       //键盘模式    1

// }Car_Ctrl_Mode_e;

// /**
//  * @brief car类结构体
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
