#ifndef _BSP_LEDS_H
#define _BSP_LEDS_H

/* Module Includes
 *
 * Do not forget that you need to enable the clock source in the RCC
 * Initialise the
 * */

#include "stm32f4xx_hal.h"


#define LED_3			(uint16_t)GPIO_PIN_13
#define LED_all_pin 	LED_3


void LEDs_init(GPIO_TypeDef * GPIO_FOR_LEDS);

void LED_toggle(uint16_t LED);

void LED_off(uint16_t LED);

void LED_on(uint16_t LED);

#endif // _BSP_LEDS_H
