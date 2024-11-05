/* Includes ------------------------------------------------------------------*/
#include "drv_tim.h"


extern TIM_HandleTypeDef htim3;

/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void PWM_Init(void)
{
	HAL_TIM_Base_Init(&htim3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
}




void Set_PWM_DutyCycle(uint32_t channel, float dutyCycle) 
{
	uint32_t period = 2000 - 1;
	
	if (dutyCycle > 100.0f)
	{
		dutyCycle = 100.0f;
	}
	else if (dutyCycle < 0.0f)
	{
		dutyCycle = 0.0f;
	}
	
	// 根据占空比计算比较值
	uint32_t compareValue = (uint32_t)(period * (dutyCycle / 100.0f));
	__HAL_TIM_SetCompare(&htim3, channel, compareValue);
	
}
