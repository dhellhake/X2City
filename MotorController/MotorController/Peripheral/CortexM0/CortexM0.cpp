/* 
* CortexM0.cpp
*
* Created: 14.11.2021 09:06:25
* Author: dominik hellhake
*/
#include "CortexM0.h"
#include <string.h>

volatile uint32_t ElapsedMilis = 0;
volatile struct os_task *os_curr_task;
volatile struct os_task *os_next_task;

void SysTick_Handler(void);

void InitSysTick()
{
	SysTick->CTRL =		0;				// Disable SysTick
	NVIC_SetPriority(SysTick_IRQn, 0);	// Set interrupt priority to highest urgency
	SysTick->LOAD =		48000;			// Set reload register for overflow interrupts
	SysTick->VAL =		0;				// Reset the SysTick counter value
	SysTick->CTRL =		0x00000007;		// Enable SysTick, Enable SysTick Exceptions, Use CPU Clock
	NVIC_EnableIRQ(SysTick_IRQn);		// Enable SysTick Interrupt
}


static void task_finished(void *params)
{
	volatile unsigned int i = 0;
	while (1)
		i++;
}

uint8_t os_init(void)
{
	if (state != STATE_DEFAULT)
		return 0;

	memset(&task_table, 0, sizeof(task_table));
	state = STATE_INITIALIZED;

	return 1;
}

uint8_t os_task_init(void (*handler)(void *params), void *task_params, uint32_t *stack, size_t stack_size)
{
	if (state != STATE_INITIALIZED && state != STATE_TASKS_INITIALIZED)
		return 0;

	if (task_table.size >= OS_CONFIG_MAX_TASKS)
		return 0;

	if ((stack_size % sizeof(uint32_t)) != 0) /* TODO: Use assert? */
		return 0;

	uint32_t stack_offset = (stack_size/sizeof(uint32_t));

	/* Initialize the task structure and set SP to the top of the stack
	   minus 16 words (64 bytes) to leave space for storing 16 registers: */
	struct os_task *task = &task_table.tasks[task_table.size];
	task->handler = handler;
	task->params = task_params;
	task->sp = (uint32_t)(stack+stack_offset-16);
	task->status = OS_TASK_STATUS_IDLE;

	/* Save init. values of registers which will be restored on exc. return:
	   - XPSR: Default value (0x01000000)
	   - PC: Point to the handler function (with LSB masked because the
	         behavior is unpredictable if pc<0> == '1' on exc. return)
	   - LR: Point to a function to be called when the handler returns
	   - R0: Point to the handler function's parameter */
	stack[stack_offset-1] = 0x01000000;
	stack[stack_offset-2] = (uint32_t)handler & ~0x01UL;
	stack[stack_offset-3] = (uint32_t)&task_finished;
	stack[stack_offset-8] = (uint32_t)task_params;

	state = STATE_TASKS_INITIALIZED;
	task_table.size++;

	return 1;
}

uint8_t os_start()
{
	if (state != STATE_TASKS_INITIALIZED)
		return 0;

	NVIC_SetPriority(PendSV_IRQn, 0xff); /* Lowest possible priority */

	/* Start the first task: */
	os_curr_task = &task_table.tasks[task_table.current_task];
	state = STATE_STARTED;

	__set_PSP(os_curr_task->sp+64); /* Set PSP to the top of task's stack */
	__set_CONTROL(0x03); /* Switch to Unprivilleged Thread Mode with PSP */
	__ISB(); /* Execute ISB after changing CONTORL (recommended) */

	os_curr_task->handler(os_curr_task->params);

	return 1;
}

void SysTick_Handler()
{
	ElapsedMilis++;
	
	os_curr_task = &task_table.tasks[task_table.current_task];
	os_curr_task->status = OS_TASK_STATUS_IDLE;

	/* Select next task: */
	task_table.current_task++;
	if (task_table.current_task >= task_table.size)
		task_table.current_task = 0;

	os_next_task = &task_table.tasks[task_table.current_task];
	os_next_task->status = OS_TASK_STATUS_ACTIVE;

	/* Trigger PendSV which performs the actual context switch: */
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}