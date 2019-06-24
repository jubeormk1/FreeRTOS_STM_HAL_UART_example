#ifndef _BSP_BUTTON_H
#define _BSP_BUTTON_H

#include "stm32f4xx_hal.h"

void BUTTON_init(void);

void BUTTON_attachInterrupt(void (buttonInterruptISR) (void),
							uint32_t PreempPrior );

GPIO_PinState BUTTON_read(void);

#endif // _BSP_BUTTON_H
