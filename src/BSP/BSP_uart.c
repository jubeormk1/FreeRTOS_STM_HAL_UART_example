#include "BSP_uart.h"

/*
 * TODO: Implementar IRQHandler
 * TODO: Test de envio y recepcion fuera de RTOS
 * */

/*=============================================================================
 *
 *=============================================================================
 */
#ifdef NUCLEOF401RE /* port para NucleoF401RE */

#define PC_UART_ENABLE()		__HAL_RCC_USART2_CLK_ENABLE()
#define PC_UART_INSTANCE		USART2

#define PC_BAUD_RATE				9600
#define PC_UART_SubPriority		(uint32_t)0

#define GPIO_PC_UART_ENABLE()	__HAL_RCC_GPIOD_CLK_ENABLE()
#define PC_UART_PORT			GPIOA
#define PC_UART_TX_pin			GPIO_PIN_2
#define PC_UART_RX_pin			GPIO_PIN_3

#else
#error UART BSP not defined for this board
#endif


/*=============================================================================
 *
 *=============================================================================
 */
UART_HandleTypeDef huart_pc;

uint32_t pc_uart_txCount = 0, pc_uart_rxCount = 0, pc_uart_errorCount = 0;

static void (*uart_rx_irq)(void);


/*=============================================================================
 *
 *=============================================================================
 */

static void rx_interrupt_init(	USART_TypeDef * usart_instance,
								uint32_t PreempPrior);

void HAL_UART_MspInit(UART_HandleTypeDef * huart);

void HAL_UART_MspDeInit(UART_HandleTypeDef * huart);


/*=============================================================================
 *
 *=============================================================================
 */

void BSP_pc_uart_init(void)
{
/* Primero la inicializacion del UART y en el Msp la inicializacion del GPIO*/

	PC_UART_ENABLE();

	huart_pc.Init.BaudRate 	= PC_BAUD_RATE;
	huart_pc.Init.HwFlowCtl 	= UART_HWCONTROL_NONE;
	huart_pc.Init.Mode 		= UART_MODE_TX_RX;
	huart_pc.Init.OverSampling = UART_OVERSAMPLING_8;
	huart_pc.Init.Parity 	= UART_PARITY_NONE;
	huart_pc.Init.StopBits	= UART_STOPBITS_1;
	huart_pc.Init.WordLength = UART_WORDLENGTH_8B;
	huart_pc.Instance = PC_UART_INSTANCE;

	HAL_UART_Init(&huart_pc);

}


void BSP_pc_uart_deinit(void)
{
	HAL_UART_DeInit(&huart_pc);
}


uint16_t BSP_pc_uart_tx(uint8_t * pData, uint16_t size)
{
	uint16_t retVal = 0;
#if 1
	if(HAL_UART_Transmit_IT(&huart_pc, pData, size)== HAL_OK)
		retVal = 1;

#else

	if(HAL_UART_Transmit(&huart_pc, pData, size,HAL_MAX_DELAY)== HAL_OK)
		retVal = 1;

#endif
	return retVal;
}

void BSP_pc_uart_rx_attach_IRQ( void irq_fun_rx (void), uint32_t priority)
{
	/* La recepcion la iniciamos con BSP_uart_rx_start */
	uart_rx_irq = irq_fun_rx;

	rx_interrupt_init(PC_UART_INSTANCE, priority);
}


uint16_t BSP_pc_uart_rx_start(	uint8_t * pbuffer, uint16_t len)
{
	uint16_t retVal = 0;
	if(HAL_UART_Receive_IT(&huart_pc, pbuffer, len)== HAL_OK)
		retVal = 1;

	return retVal;
}


/*=============================================================================
 *
 *============================================================================*/

static void rx_interrupt_init(	USART_TypeDef * usart_instance,
								uint32_t PreempPrior)
{
	if (usart_instance == PC_UART_INSTANCE)
	{
		HAL_NVIC_SetPriority(USART2_IRQn, PreempPrior, PC_UART_SubPriority);

		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
}

/*=============================================================================
 *
 *============================================================================*/

void HAL_UART_MspInit(UART_HandleTypeDef * huart)
{
	GPIO_InitTypeDef hinitgpio;

	if(huart->Instance == PC_UART_INSTANCE)
	{
		GPIO_PC_UART_ENABLE();

		hinitgpio.Pull = GPIO_PULLUP;

		hinitgpio.Mode = GPIO_MODE_AF_PP;

		hinitgpio.Alternate = GPIO_AF7_USART2;

		hinitgpio.Pin = PC_UART_RX_pin | PC_UART_TX_pin;

		HAL_GPIO_Init(PC_UART_PORT, &hinitgpio);

//		__HAL_UART_ENABLE_IT(huart, UART_IT_TC);
//
//		__HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

		HAL_NVIC_SetPriority(USART2_IRQn, 13, PC_UART_SubPriority);

		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef * huart)
{
	if(huart->Instance == PC_UART_INSTANCE)
	{
		HAL_GPIO_DeInit(PC_UART_PORT, PC_UART_RX_pin | PC_UART_TX_pin);
	}
}


/*=============================================================================
 *
 *============================================================================*/


void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart_pc);
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == PC_UART_INSTANCE)
	{
		pc_uart_txCount++;

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == PC_UART_INSTANCE)
	{
		pc_uart_rxCount++;

		if(uart_rx_irq != NULL)
		{
			uart_rx_irq();
		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == PC_UART_INSTANCE)
	{
		pc_uart_errorCount++;

//		reset_uart(huart);
	}
}
