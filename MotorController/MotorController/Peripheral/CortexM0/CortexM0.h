/* 
* CortexM0.h
*
* Created: 14.11.2021 09:06:25
*  Author: Dominik Hellhake
*/
#ifndef __CORTEXM0_H__
#define __CORTEXM0_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "samc21.h"
	#include <stddef.h>

	#define OS_CONFIG_MAX_TASKS 3

	enum os_task_status {
		OS_TASK_STATUS_IDLE = 1,
		OS_TASK_STATUS_ACTIVE,
	};

	struct os_task {
		volatile uint32_t sp;
		void (*handler)(void *params);
		void *params;
		volatile enum os_task_status status;
	};

	static enum state {
		STATE_DEFAULT = 1,
		STATE_INITIALIZED,
		STATE_TASKS_INITIALIZED,
		STATE_STARTED,
	} state = STATE_DEFAULT;

	static struct task_table {
		struct os_task tasks[OS_CONFIG_MAX_TASKS];
		volatile size_t current_task;
		size_t size;
	} task_table;


	extern volatile uint32_t ElapsedMilis;
	extern volatile struct os_task *os_curr_task;
	extern volatile struct os_task *os_next_task;

	void InitSysTick();
	
	uint8_t os_init(void);
	uint8_t os_task_init(void (*handler)(void *params), void *task_params, uint32_t *stack, size_t stack_size);
	uint8_t os_start();

#ifdef __cplusplus
}
#endif

#endif //__CORTEXM0_H__
