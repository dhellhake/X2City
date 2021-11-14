/*
 * os.h
 *
 * Created: 14.11.2021 14:10:24
 *  Author: dominik hellhake
 */
#ifndef OS_H_
#define OS_H_
	
#ifdef __cplusplus
extern "C" {
#endif
	
	#include <stddef.h>
	#include "samc21.h"
	
	#define OS_CONFIG_MAX_TASKS 4

	typedef enum os_state 
	{
		STATE_DEFAULT				= 0x1,
		STATE_INITIALIZED			= 0x2,
		STATE_TASKS_INITIALIZED		= 0x3,
		STATE_STARTED				= 0x4,
	} os_state;
		
	typedef enum os_task_status 
	{
		OS_TASK_STATUS_READY		= 0x1,
		OS_TASK_STATUS_ACTIVE		= 0x2,
		OS_TASK_STATUS_SUSPENDED	= 0x3,
		OS_TASK_STATUS_COMPLETE		= 0x4,
	} os_task_status;

	typedef enum os_task_cycle_time
	{
		OS_TASK_CYCLE_TIME_MANUAL	= 0x0,
		OS_TASK_CYCLE_TIME_10MS		= 0x1,
		OS_TASK_CYCLE_TIME_50MS		= 0x2,
		OS_TASK_CYCLE_TIME_100MS	= 0x4,		
	} os_task_cycle_time;
	
	typedef struct os_task 
	{
		volatile uint32_t				sp;
		void							(*handler)(void *params);
		void*							executable;
		volatile os_task_status			status;
		volatile os_task_cycle_time		cycleTime;
		uint32_t*						stack;
		uint32_t						stackSize;
	} os_task;

	extern os_task tasks[OS_CONFIG_MAX_TASKS];
	extern uint8_t currentTaskIdx;
	extern os_state OS_STATE;

	uint8_t os_task_reset(os_task* task);
	uint8_t os_start();
	
	void os_task_scheduler();

#ifdef __cplusplus
}
#endif

#endif /* OS_H_ */