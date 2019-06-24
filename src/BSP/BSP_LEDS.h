#ifndef _BSP_LEDS_H
#define _BSP_LEDS_H

/* Module Includes
 *
 * Do not forget that you need to enable the clock source in the RCC
 * Initialise the
 * */

#include "stm32f4xx_hal.h"


#define LED_2			(uint16_t)GPIO_PIN_5
#define LED_all_pin 	LED_2


void BSP_leds_init(void);

void BSP_led_toggle(uint16_t LED);

void BSP_led_off(uint16_t LED);

void BSP_led_on(uint16_t LED);

#endif // _BSP_LEDS_H
