/*
 * os.c
 *
 * Created: 18.12.2022 13:35:00
 *  Author: dominik hellhake
 */ 
#include "os.h"
#include <string.h>

#define COMPILER_ALIGNED(a)				__attribute__((__aligned__(a)))

volatile uint32_t elapsed_100us = 0;

typedef enum os_task_status
{
	OS_TASK_STATUS_PREINIT		= 0,
	OS_TASK_STATUS_SUSPENDED	= 1,
	OS_TASK_STATUS_READY		= 2,
	OS_TASK_STATUS_ACTIVE		= 3,
	OS_TASK_STATUS_FINISHED		= 4,
	OS_TASK_STATUS_ERROR		= 255,
} os_task_status;

typedef struct os_task
{
	volatile uint32_t sp;
	void (*cyclic)(uint32_t tstmp_ms);
	uint32_t (*init)(void);
	volatile os_task_status status;
	os_cycle_time cycle_time;
	uint32_t stack[OS_CONFIG_STACK_SIZE / 4];
} os_task;

volatile os_task *os_curr_task;
volatile os_task *os_next_task;


COMPILER_ALIGNED(4)
static struct task_table 
{
	struct os_task tasks[OS_CONFIG_MAX_TASKS];
	volatile size_t current_task_idx;
} task_table;


static void cyclic_handler(void *params)
{
	os_task *task = params;
	
	while (1)
	{
		if (task->status == OS_TASK_STATUS_PREINIT)
		{
			if (task->init() > 0)
				task->status = OS_TASK_STATUS_READY;
			else
				task->status = OS_TASK_STATUS_ERROR;
		} else if (task->status != OS_TASK_STATUS_FINISHED)
		{
			task->cyclic(GetElapsedMilis());
			task->status = OS_TASK_STATUS_FINISHED;
		}		
	}	
}

void os_init(void)
{	
	SysTick->CTRL =		0;										// Disable SysTick
	SysTick->LOAD =		4807;									// Set reload register for overflow interrupts
	SysTick->VAL =		0;										// Reset the SysTick counter value
	SysTick->CTRL  =	(1 << SysTick_CTRL_CLKSOURCE_Pos) |		/* Use Processor Clock */
						(1 << SysTick_CTRL_TICKINT_Pos)   |		/* Interrupt enable */
						(1 << SysTick_CTRL_ENABLE_Pos);			/* SysTick enable */
	NVIC_SetPriority(SysTick_IRQn, 0);							// Set interrupt priority to highest urgency
		
	memset(&task_table, 0, sizeof(task_table));
}

void os_task_init(void (*cyclic)(uint32_t tstmp_ms), uint32_t (*init)(void), uint32_t table_index, os_cycle_time cycle_time)
{
	uint32_t stack_offset = (OS_CONFIG_STACK_SIZE/sizeof(uint32_t));

	os_task *task = &task_table.tasks[table_index];
	task->cyclic = cyclic;
	task->init = init;
	task->sp = (uint32_t)(task->stack + stack_offset - 16);
	task->cycle_time = cycle_time;
	
	if (cycle_time == OS_CONFIG_CYCLE_TIME_NONCYCLIC)
		task->status = OS_TASK_STATUS_SUSPENDED;
	else
		task->status = OS_TASK_STATUS_PREINIT;

	task->stack[stack_offset-1] = 0x01000000;
	task->stack[stack_offset-2] = (uint32_t)cyclic_handler & ~0x01UL;
	task->stack[stack_offset-3] = (uint32_t)0x00000000;
	task->stack[stack_offset-8] = (uint32_t)task;
}

void os_start()
{
	NVIC_SetPriority(PendSV_IRQn, 0xff);	/* Lowest possible priority */
	NVIC_EnableIRQ(SysTick_IRQn);			// Enable SysTick Interrupt

	/* Start the last task in table */
	task_table.current_task_idx = OS_CONFIG_MAX_TASKS - 1;
	os_curr_task = &task_table.tasks[task_table.current_task_idx];
	
	__set_PSP(os_curr_task->sp+64); /* Set PSP to the top of task's stack */
	__set_CONTROL(0x03); /* Switch to Unprivilleged Thread Mode with PSP */
	__ISB(); /* Execute ISB after changing CONTORL (recommended) */

	os_curr_task->status = OS_TASK_STATUS_ACTIVE;
	cyclic_handler((void*)os_curr_task);

	return;
}

void SetTaskSwitch(uint32_t current_task_idx, uint32_t next_task_idx)
{
	os_curr_task = &task_table.tasks[current_task_idx];
	if (os_curr_task->status != OS_TASK_STATUS_FINISHED)
		os_curr_task->status = OS_TASK_STATUS_SUSPENDED;
	
	task_table.current_task_idx = next_task_idx;
	os_next_task = &task_table.tasks[task_table.current_task_idx];
	
	if (os_next_task->status != OS_TASK_STATUS_PREINIT)
		os_next_task->status = OS_TASK_STATUS_ACTIVE;
	
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
}

uint32_t GetElapsedMilis()
{
	return elapsed_100us / 10;	
}

void SysTick_Handler(void)
{	
	elapsed_100us++;
	
	/* Reset Watchdog */	
	WDT->CLEAR.reg = WDT_CLEAR_CLEAR_KEY;

	/* Check for tasks beeing ready for execution */
	for (int taskIdx = 0; taskIdx < OS_CONFIG_MAX_TASKS; taskIdx++)
	{
		if (task_table.tasks[taskIdx].cycle_time != OS_CONFIG_CYCLE_TIME_NONCYCLIC &&
			task_table.tasks[taskIdx].status == OS_TASK_STATUS_FINISHED &&
			((elapsed_100us % (task_table.tasks[taskIdx].cycle_time * 10)) == 0))	// Check for finished and cycletime
		{
				if (taskIdx != task_table.current_task_idx)
					task_table.tasks[taskIdx].status = OS_TASK_STATUS_READY;		// Set ready
		}
	}
			
	os_task *curr_task = &task_table.tasks[task_table.current_task_idx];
	if (curr_task->cycle_time == OS_CONFIG_CYCLE_TIME_NONCYCLIC ||		// Ready for Task-Switch 
		curr_task->status == OS_TASK_STATUS_FINISHED) 
	{		
		for (int taskIdx = 0; taskIdx < OS_CONFIG_MAX_TASKS; taskIdx++)
		{  
			if (taskIdx != task_table.current_task_idx)
			{
				switch(task_table.tasks[taskIdx].status)
				{
					case OS_TASK_STATUS_PREINIT:	// Start Uninitialized
					case OS_TASK_STATUS_READY:		// Start ready
					case OS_TASK_STATUS_SUSPENDED:	// Resume suspended task
						SetTaskSwitch(task_table.current_task_idx, taskIdx);
						taskIdx = OS_CONFIG_MAX_TASKS;
					break;
					default:
					break;
				}
			}
		}		
	}
}

