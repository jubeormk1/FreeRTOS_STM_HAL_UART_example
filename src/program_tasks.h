#ifndef _TASKS_H
#define _TASKS_H

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "list.h"
#include "semphr.h"

#include "BSP_LEDS.h"

typedef struct {
	uint16_t led_assigned;
	TaskHandle_t * next_task_notify;
}xLEDTaskParameters_t;

/* API tareas */
void task_notify_button_press(void);

void task_update_rtos_SysTick(void);

void task_create_tasks(void);

void task_init_scheduler(void);

#endif // _TASKS_H
