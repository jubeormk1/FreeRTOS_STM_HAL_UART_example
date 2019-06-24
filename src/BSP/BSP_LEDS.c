#include "BSP_LEDS.h"

#ifdef NUCLEOF401RE

#define LED_PORT		GPIOA

#define IS_LED(LED)    ((LED)	== LED_2	)

#define GPIO_LED_ENABLE()		__HAL_RCC_GPIOA_CLK_ENABLE()

#else
#error BSP LED no listo para esta plataforma
#endif

/**/
GPIO_TypeDef * LEDs_GPIO_port = LED_PORT;


void BSP_leds_init(void)
{
	GPIO_LED_ENABLE();

	GPIO_InitTypeDef gpioInit;

	gpioInit.Mode = GPIO_MODE_OUTPUT_PP;
	gpioInit.Pull = GPIO_NOPULL;
	gpioInit.Speed = GPIO_SPEED_FAST;

	gpioInit.Pin = LED_all_pin;

	HAL_GPIO_Init(LEDs_GPIO_port, &gpioInit);

}


void BSP_led_toggle(uint16_t LED)
{
	assert_param(IS_LED(LED));

	HAL_GPIO_TogglePin(LED_PORT, LED);

}


void BSP_led_off(uint16_t LED)
{
	assert_param(IS_LED(LED));

	HAL_GPIO_WritePin(LED_PORT, LED, GPIO_PIN_RESET);
}

void BSP_led_on(uint16_t LED)
{
	assert_param(IS_LED(LED));

	HAL_GPIO_WritePin(LED_PORT, LED, GPIO_PIN_SET);
}
