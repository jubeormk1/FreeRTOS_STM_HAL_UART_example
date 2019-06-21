#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "stm32f4xx_hal.h"

void BSP_uart_init(void);

void BSP_uart_deinit(void);

void BSP_uart_tx(void);

void BSP_uart_rx(void);

#endif // _BSP_UART_H
