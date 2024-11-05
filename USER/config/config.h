/**
  ******************************************************************************
  * @file           : RP_drv
  * @brief          : 
  * @note           : start 2022-11-3 
  ******************************************************************************
  */

#ifndef __CONFIG_H
#define __CONFIG_H

#include "main.h"

#define c_sign(x)										((x>=0)?(1):(-1))//符号
#define constrain(x, min, max)			((x>(max))?max:(x<(min)?min:(x)))//限幅
#define range(x, min, max)					((((x)<(max)) && ((x)>(min)))?1:0)//范围
#define c_abs(x) 										((x)>0? (x):(-(x)))//绝对值
#define	c_max2(x,y)									(c_abs(x)>c_abs(y)?x:y)//下限
#define distance(x,y)								((x>=y)?(x-y):(y-x))//距离
#define	d2r_f32(x) 									(0.01745329f*(x))//度to弧度
#define	r2d_f32(x) 									(57.29578f*(x))//弧度to度
#define	pi													(3.141592653f)

//	低通滤波  x=当前  x_=过去  a=一阶低通系数
#define low_pass(x,x_,a)						((x_)+(((x)-(x_))*(a)))//a越大越信任当前
//奇异数据筛除
#define EliminateSingularData(x,x_,err_max)					(c_abs(((x)-(x_)))>(err_max)?(x_):(x))
/******************************全局使用枚举******************************/

typedef enum DEV_WORK_STATE
{
	DEV_OFFLINE = 0,
	DEV_ONLINE,
	
	DEV_NO = 0,
	DEV_OK,
}Dev_Work_State_e;

/**
 * @brief 未初始化：DEV_RESET_NO 初始化完成:DEV_RESET_OK
 * 
 */
typedef enum DEV_RESET_STATE
{
	DEV_RESET_NO,
	DEV_RESET_OK,
}Dev_Reset_State_e;

#endif
