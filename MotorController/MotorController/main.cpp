/*
 * MotorController.cpp
 *
 * Created: 14.11.2021 08:55:40
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "Peripheral/CortexM0/CortexM0.h"

static void task_idle(void *params);
static void task_handler1(void *params);
static void task_handler2(void *params);

int main(void)
{
	uint8_t status;
	
	/* Initialize task stacks: */
	static uint32_t stackIdle[128];
	static uint32_t stack1[128];
	static uint32_t stack2[128];
	
	status = os_init();
	status = 0;
	status = os_task_init(&task_idle, (void*)status, stackIdle, sizeof(stackIdle));
	status = 1;
	status = os_task_init(&task_handler1, (void*)status, stack1, sizeof(stack1));
	status = 2;
	status = os_task_init(&task_handler2, (void*)status, stack2, sizeof(stack2));
	
	/* Context switch every second: */
	status = os_start();
	
    while (1) {}
}

static void task_idle(void *params)
{
	while (1);
}

static void task_handler1(void *params)
{
	uint32_t milis;
	while (1)
	{
		PORT->Group[0].OUTTGL.reg = PORT_PA13;
		
		
		milis = ElapsedMilis + 50;
		while (ElapsedMilis < milis);
	}
}

static void task_handler2(void *params)
{
	uint32_t milis;
	while (1)
	{
		PORT->Group[0].OUTTGL.reg = PORT_PA12;
		
		
		milis = ElapsedMilis + 100;
		while (ElapsedMilis < milis);
	}
}