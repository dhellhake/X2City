/*
 * HMI.c
 *
 * Created: 20.05.2023 10:57:14
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "OS/os.h"
#include "Com/Com.h"
#include "Peripheral/CAN/CANlib.h"


static void background(uint32_t tstmp_ms)
{
	while(1);
}

int main(void)
{
	os_init();
	
	os_task_init(&Com_Rx_Cyclic, &Com_Init, 0, OS_CONFIG_CYCLE_TIME_10MS);
	
	os_task_init(&background, 0x00000000, 1, OS_CONFIG_CYCLE_TIME_NONCYCLIC);

	os_start();
	
	while (1);
}
