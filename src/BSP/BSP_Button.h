#ifndef _BSP_BUTTON_H
#define _BSP_BUTTON_H

#include "stm32f4xx_hal.h"

typedef enum
{
	EDGE_RISING, 				/* Interrupcion por detección de flanco ascendente */
	EDGE_FALLING, 			/* Interrupcion por detección de Flanco descendente */
	EDGE_RISING_FALLING /* Interrupcion por detección de flanco ascendente o descendente */
}edge_sel_t;


void BUTTON_init(void);

void BUTTON_attachInterrupt(void (buttonInterruptISR) (void),
							uint32_t PreempPrior );

GPIO_PinState BUTTON_read(void);

#endif // _BSP_BUTTON_H
