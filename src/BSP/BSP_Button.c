#include "BSP_Button.h"

#ifdef NUCLEOF401RE

#define BUTTON_PORT					GPIOC
#define BUTTON_PIN					GPIO_PIN_13
#define BUTTON_IRQn					EXTI15_10_IRQn
#define BUTTON_PreemptPriority		(uint32_t)
#define BUTTON_SubPriority			(uint32_t)0
#define GPIO_BUTTON_ENABLE()		__HAL_RCC_GPIOC_CLK_ENABLE()

#else
 #error BSP boton no listo para plataforma
#endif

static void (* buttonISR_h) (void);

void EXTI15_10_IRQHandler(void);

void BSP_btn_init (void)
{
	GPIO_BUTTON_ENABLE();

	GPIO_InitTypeDef buttonInit;

	buttonInit.Mode = GPIO_MODE_IT_FALLING;

	buttonInit.Pull = GPIO_NOPULL;

	buttonInit.Pin = BUTTON_PIN;

	HAL_GPIO_Init(BUTTON_PORT, &buttonInit);
}


void BSP_btn_attachInterrupt(void (buttonInterruptISR) (void), uint32_t PreempPrior )
{
	buttonISR_h = buttonInterruptISR;

	HAL_NVIC_SetPriority(BUTTON_IRQn, PreempPrior, BUTTON_SubPriority);

	HAL_NVIC_EnableIRQ(BUTTON_IRQn);
}


GPIO_PinState BSP_btn_read( void )
{
	GPIO_PinState retval = GPIO_PIN_RESET;

	retval = HAL_GPIO_ReadPin(BUTTON_PORT,BUTTON_PIN);

	return retval;
}


void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(BUTTON_PIN);
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin) {
		case BUTTON_PIN:
			if (buttonISR_h != NULL)
			{
				buttonISR_h();
			}
			break;
		default:
			break;
	}
}
