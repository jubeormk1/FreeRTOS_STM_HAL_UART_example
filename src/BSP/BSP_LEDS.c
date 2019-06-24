#include "BSP_LEDS.h"

#ifdef NUCLEOF401RE

#define LED_PORT		GPIOD

#define IS_LED(LED)    ((LED)	== LED_3	)

#define GPIO_LED_ENABLE()		__HAL_RCC_GPIOD_CLK_ENABLE()

#else
#error BSP LED no listo para esta plataforma
#endif

/**/
GPIO_TypeDef * LEDs_GPIO_port;


void LEDs_init(void)
{
	GPIO_LED_ENABLE();

	GPIO_InitTypeDef gpioInit;

	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FAST;

	gpioInit.Pin |= LED_all_pin;

	HAL_GPIO_Init(LEDs_GPIO_port, &gpioInit);

}


void LED_toggle(uint16_t LED)
{
	assert_param(IS_LED(LED));

	HAL_GPIO_TogglePin(LED_PORT, LED);

}


void LED_off(uint16_t LED)
{
	assert_param(IS_LED(LED));

	HAL_GPIO_WritePin(LED_PORT, LED, GPIO_PIN_RESET);
}

void LED_on(uint16_t LED)
{
	assert_param(IS_LED(LED));

	HAL_GPIO_WritePin(LED_PORT, LED, GPIO_PIN_SET);
}
