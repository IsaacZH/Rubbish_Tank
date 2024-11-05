#include "drive.h"


void Drive_Init(void)
{

	USART1_Init();
	USART2_Init();
	USART3_Init();
	USART4_Init();
	PWM_Init();
}


