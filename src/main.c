/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "BSP_uart.h"
#include "BSP_Button.h"
#include "BSP_LEDS.h"

#include "string.h"
// ----------------------------------------------------------------------------
//
// STM32F4 empty sample (trace via NONE).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the NONE output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

#define RX_BUFFER_LEN 10

char last_rx_byte = 0;

char rx_buffer[RX_BUFFER_LEN];
uint16_t	rx_buffer_index = 0;

void SysTick_Handler (void);

void trigger_TX_pc(void);

void receive_RX(void);

const	uint8_t uart_send[] = "Mandando una simple cadena de texto\r\n";
// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[])
{
	// At this stage the system clock should have already been configured
	// at high speed.

	HAL_Init();

	BSP_leds_init();

	BSP_btn_init();

	BSP_btn_attachInterrupt(trigger_TX_pc,15);

	BSP_pc_uart_init(); /* Aun no tengo la interrupcion activada*/

	BSP_pc_uart_rx_attach_IRQ(receive_RX, 15);

	BSP_pc_uart_rx_start(&last_rx_byte,1);

	// Infinite loop
	while (1)
	{
		HAL_Delay(1000);
		//LED_toggle(LED_2);
		// Add your code here.
	}
}


void trigger_TX_pc(void)
{
	BSP_pc_uart_tx(uart_send, strlen(uart_send));
	BSP_led_toggle(LED_2);
}

void receive_RX(void)
{
	char received_byte = last_rx_byte;
	rx_buffer[rx_buffer_index++] = received_byte;
//	BSP_pc_uart_tx(rx_buffer[rx_buffer_index-1], 1);
	if (received_byte == '\n' ||
			received_byte == '\r' ||
			rx_buffer_index >= (RX_BUFFER_LEN-2))
	{
		BSP_led_toggle(LED_2);
		rx_buffer[rx_buffer_index++] = '\n';
		rx_buffer[rx_buffer_index++] = '\r';
		BSP_pc_uart_tx(rx_buffer, rx_buffer_index);
		rx_buffer_index = 0;
	}
	BSP_pc_uart_rx_start(&last_rx_byte,1);
}

void SysTick_Handler (void)
{
	HAL_IncTick();

//	task_update_rtos_SysTick();

}
#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
