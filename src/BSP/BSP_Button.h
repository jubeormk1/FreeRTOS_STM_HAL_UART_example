#ifndef _BSP_BUTTON_H
#define _BSP_BUTTON_H

#include "stm32f4xx_hal.h"

void BSP_btn_init(void);

void BSP_btn_attachInterrupt(void (buttonInterruptISR) (void),
							uint32_t PreempPrior );

GPIO_PinState BSP_btn_read(void);

#endif // _BSP_BUTTON_H
