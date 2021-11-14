/*
 * os.c
 *
 * Created: 14.11.2021 14:51:05
 *  Author: dominik hellhake
 */ 
#include "os.h"
#include "..\Peripheral\CortexM0\CortexM0.h"


os_task tasks[OS_CONFIG_MAX_TASKS];
os_state OS_STATE = STATE_DEFAULT;
uint8_t currentTaskIdx = 0;


void task_finished(void *params)
{
	while (1);
}

uint8_t os_task_reset(os_task* task)
{
	// Clear Stack
	for (uint16_t x = 0; x < task->stackSize/4; x++)
		task->stack[x] = 0;
	
	uint32_t stack_offset = (task->stackSize/sizeof(uint32_t));

	/* Initialize the task structure and set SP to the top of the stack
		minus 16 words (64 bytes) to leave space for storing 16 registers: */
	task->sp = (uint32_t)(task->stack + stack_offset - 16);
	task->status = OS_TASK_STATUS_READY;
	
	task->stack[stack_offset-1] = 0x01000000;							// XPSR: Default value (0x01000000)
	task->stack[stack_offset-2] = (uint32_t)task->handler & ~0x01UL;	//   PC: Point to the handler function
	task->stack[stack_offset-3] = (uint32_t)&task_finished;				//   LR: Point to a function to be called when the handler returns
	task->stack[stack_offset-8] = (uint32_t)task;						//   R0: Point to the handler function's parameter

	return 1;
}

uint8_t os_start()
{
	NVIC_SetPriority(PendSV_IRQn, 0xff); /* Lowest possible priority */

	/* Start the first task: */
	os_curr_sp = &tasks[OS_CONFIG_MAX_TASKS - 1].sp;
	OS_STATE = STATE_STARTED;
	currentTaskIdx = OS_CONFIG_MAX_TASKS - 1;

	__set_PSP(tasks[OS_CONFIG_MAX_TASKS - 1].sp+64); /* Set PSP to the top of task's stack */
	__set_CONTROL(0x03); /* Switch to Unprivilleged Thread Mode with PSP */
	__ISB(); /* Execute ISB after changing CONTORL (recommended) */

	tasks[OS_CONFIG_MAX_TASKS - 1].handler(&tasks[OS_CONFIG_MAX_TASKS - 1]);

	return 1;
}

uint8_t GetCyclicTrigger(uint32_t elapsed_ms)
{
	uint8_t triggerTaskCycles = 0x00;
	if ((elapsed_ms % 10) == 0)
	{
		triggerTaskCycles |= (uint8_t)OS_TASK_CYCLE_TIME_10MS;
		if ((elapsed_ms % 50) == 0)
			triggerTaskCycles |= (uint8_t)OS_TASK_CYCLE_TIME_50MS;
		if ((elapsed_ms % 100) == 0)
			triggerTaskCycles |= (uint8_t)OS_TASK_CYCLE_TIME_100MS;
	}
	
	return triggerTaskCycles;
}

inline void os_task_scheduler()
{
	if (OS_STATE != STATE_STARTED)
		return;
		
	// Reset completed tasks other than currently running
	uint8_t triggerTaskCycles = GetCyclicTrigger(ElapsedMilis);
	for (uint8_t x = 0; x < OS_CONFIG_MAX_TASKS; x++)
		if (x != currentTaskIdx)
			if ((tasks[x].cycleTime & triggerTaskCycles) != 0x00)
				if (tasks[x].status == OS_TASK_STATUS_COMPLETE) //Else: MTA
					os_task_reset(&tasks[x]);
		
	// Check for higher priority tasks (ready or suspended.
	for (uint8_t x = 0; x < currentTaskIdx; x++)	
		if (tasks[x].status == OS_TASK_STATUS_READY || tasks[x].status == OS_TASK_STATUS_SUSPENDED)
		{
			tasks[currentTaskIdx].status = OS_TASK_STATUS_SUSPENDED;
			
			os_curr_sp = (volatile uint32_t*)&tasks[currentTaskIdx].sp;
			os_next_sp = (volatile uint32_t*)&tasks[x].sp;
			currentTaskIdx = x;
			tasks[x].status = OS_TASK_STATUS_ACTIVE;
			SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
			return;					
		}
	
	// Check for current task beeing complete 
	if (tasks[currentTaskIdx].status == OS_TASK_STATUS_COMPLETE)
		for (uint8_t x = currentTaskIdx; x < OS_CONFIG_MAX_TASKS; x++)
			if (tasks[x].status == OS_TASK_STATUS_READY || tasks[x].status == OS_TASK_STATUS_SUSPENDED)
			{
				os_curr_sp = (volatile uint32_t*)&tasks[currentTaskIdx].sp;
				os_next_sp = (volatile uint32_t*)&tasks[x].sp;
				currentTaskIdx = x;
				tasks[x].status = OS_TASK_STATUS_ACTIVE;
				SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;
				return;
			}
};