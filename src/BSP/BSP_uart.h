#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "stm32f4xx_hal.h"

void BSP_pc_uart_init(void);

void BSP_pc_uart_deinit(void);

uint16_t BSP_pc_uart_tx(uint8_t * pData, uint16_t size); /* IT driven */

void BSP_pc_uart_rx_attach_IRQ( void irq_fun_rx (void), uint32_t priority);

uint16_t BSP_pc_uart_rx_start(uint8_t * pbuffer, uint16_t len);

#endif // _BSP_UART_H
