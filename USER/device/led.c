
#include "led.h"

/*声明函数*/
void Led_On(led_t *led);
void Led_Off(led_t *led);
void Led_Toggle(led_t *led);
void Led_Blink(led_t *led, uint16_t frequency);

/*定义LED*************************************************************/
led_t led_red = 
{
  .state = LED_OFF,
  .port = LED_C1_GPIO_Port,
  .pin = LED_C1_Pin,
  .on = Led_On,
  .off = Led_Off,
  .toggle = Led_Toggle,
  .blink = Led_Blink,
};

led_t led_blue = 
{ 
  .state = LED_OFF,
  .port = LED_C2_GPIO_Port,
  .pin = LED_C2_Pin,
  .on = Led_On,
  .off = Led_Off,  
  .toggle = Led_Toggle,
  .blink = Led_Blink,
};

led_t led_green = 
{
  .state = LED_OFF,
  .port = LED_C3_GPIO_Port,
  .pin = LED_C3_Pin,
  .on = Led_On,
  .off = Led_Off,
  .toggle = Led_Toggle,
  .blink = Led_Blink,
};

led_t led_laser = 
{
  .state = LED_OFF,
  .port = laser_GPIO_Port,
  .pin = laser_Pin,
  .on = Led_On,
  .off = Led_Off,
  .toggle = Led_Toggle,
  .blink = Led_Blink,
};


/*LED控制函数*********************************************************/

/**
 * @brief 开灯
 * 
 * @param led 
 */
void Led_On(led_t *led)
{
  HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
  led->state = LED_ON;
}

/**
 * @brief 关灯
 * 
 * @param led 
 */
void Led_Off(led_t *led)
{
  HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
  led->state = LED_OFF;
}

/**
 * @brief 翻转
 * 
 * @param led 
 */
void Led_Toggle(led_t *led)
{
  HAL_GPIO_TogglePin(led->port, led->pin);
  led->state = !led->state;
}

/**
 * @brief 控制LED闪烁频率
 * 
 * @param led 
 * @param frequency 闪烁频率，取决于调用频率
 */
void Led_Blink(led_t *led, uint16_t frequency)
{
  if (led->blink_cnt >= frequency)
  {
    Led_Toggle(led);
    led->blink_cnt = 0;
  }
  else
  {
    led->blink_cnt++;
  }
}

