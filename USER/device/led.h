#ifndef _LED_H
#define _LED_H

#include "stm32f4xx_hal.h"
#include "gpio.h"

typedef enum
{
    LED_OFF = 0,
    LED_ON = 1
} led_state_e;

typedef struct led
{
  led_state_e state;
  GPIO_TypeDef *port;
  uint16_t pin;
  uint32_t blink_cnt;
  void (*on)(struct led *led);
  void (*off)(struct led *led);
  void (*toggle)(struct led *led);
  void (*blink)(struct led *led, uint16_t frequency);
} led_t;

extern led_t led_red;
extern led_t led_green;
extern led_t led_blue;
extern led_t led_laser;
#endif