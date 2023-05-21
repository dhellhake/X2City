/*
 * os.h
 *
 * Created: 18.12.2022 13:36:03
 *  Author: dominik hellhake
 */ 
#ifndef OS_H_
#define OS_H_

#include "sam.h"

#define OS_CONFIG_MAX_TASKS				2
#define OS_CONFIG_STACK_SIZE			512

typedef enum os_cycle_time
{
	OS_CONFIG_CYCLE_TIME_NONCYCLIC =	0,
	OS_CONFIG_CYCLE_TIME_5MS =			5,
	OS_CONFIG_CYCLE_TIME_10MS =			10,
	OS_CONFIG_CYCLE_TIME_100MS =		100,
} os_cycle_time;

void os_init(void);
void os_task_init(void (*cyclic)(uint32_t tstmp_ms), uint32_t (*init)(void), uint32_t table_index, os_cycle_time cycle_time);
void os_start();

uint32_t GetElapsedMilis();

extern volatile uint32_t elapsed_100us;

#endif /* OS_H_ */