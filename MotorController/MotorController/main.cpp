/*
 * MotorController.cpp
 *
 * Created: 14.11.2021 08:55:40
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "Peripheral/CortexM0/CortexM0.h"
#include "OS/os.h"
#include "BLDCDrive/BLDCDrive.h"
#include "HallSensor/HallSensor.h"
#include "ComPort/ComPort.h"

void task_handler(void *params);

int main(void)
{	
	/* Initialize tasks: */	
	static uint32_t stackIdle[32];
	static uint32_t stack1[64];
	static uint32_t stack2[64];
	static uint32_t stack3[64];
	
	tasks[OS_CONFIG_MAX_TASKS - 1].handler = &task_idle;
	tasks[OS_CONFIG_MAX_TASKS - 1].stack = stackIdle;
	tasks[OS_CONFIG_MAX_TASKS - 1].cycleTime = OS_TASK_CYCLE_TIME_MANUAL;
	tasks[OS_CONFIG_MAX_TASKS - 1].stackSize = sizeof(stackIdle);
		
	tasks[0].handler = &task_handler;
	tasks[0].executable = (void *)&DRV;
	tasks[0].cycleTime = OS_TASK_CYCLE_TIME_100MS;
	tasks[0].stack = stack1;
	tasks[0].stackSize = sizeof(stack1);
	
	tasks[1].handler = &task_handler;
	tasks[1].executable = (void *)&Hall;
	tasks[1].cycleTime = OS_TASK_CYCLE_TIME_50MS;
	tasks[1].stack = stack2;
	tasks[1].stackSize = sizeof(stack2);
	
	tasks[2].handler = &task_handler;
	tasks[2].executable = (void *)&Com;
	tasks[2].cycleTime = OS_TASK_CYCLE_TIME_100MS;
	tasks[2].stack = stack3;
	tasks[2].stackSize = sizeof(stack3);
	
	for (uint8_t x = 0; x < OS_CONFIG_MAX_TASKS; x++)
		os_task_reset(&tasks[x]);
		
	os_start();
	
    while (1) {}
}

void task_handler(void *params)
{
	os_task* task = (os_task*)params;
	Executable* exec = (Executable*)task->executable;
	
	exec->Run(ElapsedMilis);
	task->status = OS_TASK_STATUS_COMPLETE;
}
