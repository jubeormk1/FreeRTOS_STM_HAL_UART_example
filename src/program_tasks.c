#include "program_tasks.h"


#define TASK_DELAY_MS			500
#define QUEUE_AZUL_VERDE_L		10
#define QUEUE_VERDE_NARANJA_L	1 /* Voy a usarlo como un mailbox*/

#define TASK_DELAY_AZUL_FAST_MS	200
#define TASK_DELAY_AZUL_SLOW_MS	TASK_DELAY_AZUL_FAST_MS * 2

#define TASK_DELAY_VERDE_MS		400

/* Manejadores de tareas */
TaskHandle_t xTaskVerde ;
TaskHandle_t xTaskAzul;
TaskHandle_t xTaskNaranja;
TaskHandle_t xTaskRojo;

/* Manejador de parametros de tareas */

xLEDTaskParameters_t xGreenParameters;
xLEDTaskParameters_t xBlueParameters;
xLEDTaskParameters_t xOrangeParameters;
xLEDTaskParameters_t xRedParameters;

/* Semaforos y colas */

SemaphoreHandle_t led_port_in_use = NULL;

QueueHandle_t xQueue_azul_verde = NULL;

QueueHandle_t xQueue_verde_naranja = NULL;


/* Prototipos de tareas */

void vTareaLEDAzul(void *pvParameters);

void vTareaLEDVerde(void *pvParameters);

void vTareaLEDNaranja(void *pvParameters);

void vTareaLEDRojo(void *pvParameters);

/* Prototipos de funciones auxiliares */

static void thread_safe_LED_toggle(uint16_t LED_seleccionado);


/* API */
void task_update_rtos_SysTick(void)
{
	if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
	{
		xPortSysTickHandler();
	}
}

void task_notify_button_press(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	/* Send a signal to the tasks */
	vTaskNotifyGiveFromISR(xTaskVerde,xHigherPriorityTaskWoken);

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


void task_init_scheduler(void)
{
	if( (xTaskRojo 		!=NULL) &
		(xTaskNaranja 	!=NULL) &
		(xTaskVerde 	!=NULL) &
		(xTaskAzul 		!=NULL) )
	{
		vTaskStartScheduler();
	}
	else
	{
		/* TODO: Implement your standard error routine */
	}
}


void task_create_tasks(void)
{
	/* */

	xQueue_azul_verde = xQueueCreate(QUEUE_AZUL_VERDE_L,
									sizeof(uint16_t));

	xQueue_verde_naranja = xQueueCreate(QUEUE_VERDE_NARANJA_L,
										sizeof(uint16_t));

//	xGreenParameters.led_assigned = LED_green_pin;
//	xGreenParameters.next_task_notify = &xTaskAzul;

	xTaskCreate(	vTareaLEDVerde,
					"led verde",
					configMINIMAL_STACK_SIZE,
					(void *)&xGreenParameters,
					1, // mas prioridad que bg
					&xTaskVerde);


//	xBlueParameters.led_assigned = LED_blue_pin;
//	xBlueParameters.next_task_notify = &xTaskNaranja;

	xTaskCreate(	vTareaLEDAzul,
					"led azul",
					configMINIMAL_STACK_SIZE,
					(void *)&xBlueParameters,
					1, // mas prioridad que bg
					&xTaskAzul);

//	xOrangeParameters.led_assigned = LED_orange_pin;
//	xOrangeParameters.next_task_notify = &xTaskRojo;

	xTaskCreate(	vTareaLEDNaranja,
					"led naranja",
					configMINIMAL_STACK_SIZE,
					(void *)&xOrangeParameters,
					1, // mas prioridad que bg
					&xTaskNaranja);

//	xRedParameters.led_assigned = LED_red_pin;
//	xRedParameters.next_task_notify = NULL;

	xTaskCreate(	vTareaLEDRojo,
					"led rojo",
					configMINIMAL_STACK_SIZE,
					(void *)&xRedParameters,
					1, // mas prioridad que bg
					&xTaskRojo);

}

// ----- Declaracion de tareas ------------------------------------------------

void vTareaLEDAzul(void * pvParameters)
{

	TickType_t wait_delay = pdMS_TO_TICKS(TASK_DELAY_AZUL_SLOW_MS);

	TickType_t xlastWakeTime = xTaskGetTickCount();
	uint16_t valor = 100;
	while (1) {
//		thread_safe_LED_toggle(LED_blue_pin);

		if (valor<2000)
		{
			valor = valor +10;
		}
		else
		{
			valor = 100;
		}

		if(uxQueueSpacesAvailable(xQueue_azul_verde)==0)
		{
			wait_delay = pdMS_TO_TICKS(TASK_DELAY_AZUL_SLOW_MS);
		}
		else
		{
			xQueueSend(xQueue_azul_verde, &valor, 0);
			wait_delay = pdMS_TO_TICKS(TASK_DELAY_AZUL_FAST_MS);
		}

		vTaskDelayUntil(&xlastWakeTime, wait_delay);
	}
}

void vTareaLEDVerde(void * pvParameters)
{

	const TickType_t wait_delay = pdMS_TO_TICKS(TASK_DELAY_VERDE_MS);

	uint8_t  array_index = 0;

	uint16_t array_arrive[10];



	TickType_t xlastWakeTime = xTaskGetTickCount();

	while (1) {
//		thread_safe_LED_toggle(LED_green_pin);
		// Voy a usar la cola como un mailbox. Por eso usare xQueueOverwrite
		while(xQueueReceive(xQueue_azul_verde,
							&array_arrive[array_index],0
							)==pdPASS)
		{
			array_index++;
			if(array_index >= 10)
			{
				array_index = 0;

				for(int i = 1; i<10; i++)
				{
					array_arrive[0] += array_arrive[i];
				}

				array_arrive[0] /= 10;

				xQueueOverwrite(xQueue_verde_naranja,&array_arrive[0]);
			}
		}

		vTaskDelayUntil(&xlastWakeTime,wait_delay);
	}
}


void vTareaLEDNaranja(void * pvParameters)
{

	TickType_t wait_delay = pdMS_TO_TICKS(TASK_DELAY_MS);
	uint16_t	received_delay_MS = 0;
	//TickType_t xlastWakeTime = xTaskGetTickCount();

	while (1) {
//		thread_safe_LED_toggle(LED_orange_pin);
		// Voy a usar la cola como un mailbox. Por eso usare xQueuePeek
		if(xQueuePeek(xQueue_verde_naranja,&received_delay_MS,0)==pdPASS)
		{
			wait_delay = pdMS_TO_TICKS((TickType_t)received_delay_MS);
		}
		vTaskDelay(wait_delay);
	}
}

void vTareaLEDRojo(void * pvParameters)
{

	const TickType_t wait_delay = pdMS_TO_TICKS(TASK_DELAY_MS);

	//TickType_t xlastWakeTime = xTaskGetTickCount();

	while (1) {
//		thread_safe_LED_toggle(LED_red_pin);

		vTaskDelay(wait_delay);
	}
}

/* Declaracion de funciones auxiliares ---------------------------------------*/

static void thread_safe_LED_toggle(uint16_t LED_seleccionado)
{

	if (led_port_in_use == NULL)
	{
		led_port_in_use = xSemaphoreCreateMutex();
	}

	xSemaphoreTake(led_port_in_use, portMAX_DELAY);

	LED_toggle(LED_seleccionado);

	xSemaphoreGive(led_port_in_use);
}
