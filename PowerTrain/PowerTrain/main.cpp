/*
 * PowerTrain.cpp
 *
 * Created: 01.05.2022 13:03:59
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "RuntimeEnvironment/RuntimeEnvironment.h"
#include "DeviceDriver/TC/TClib.h"
#include "ComHandler/ComHandler.h"
#include "BLDC/BLDC.h"
#include "HallSensor/HallSensor.h"

#define TASKPOOL_SIZE		3

Task* taskPool[TASKPOOL_SIZE] = {
	&DRV,
	&Hall,
	&ComHdl
};
uint16_t timeSlot[TASKPOOL_SIZE]
{
	2000,
	4000,
	4000
};

int main(void)
{
	uint32_t sz = sizeof(rte_image_record_t);
	
	uint64_t t_now = 0;
	uint64_t t_now_2 = 0;
	uint8_t taskIndex = 0;
	uint32_t runtimeRecord;
	
	while (1)
	{
		t_now = GetElapsedMicros();
		taskPool[taskIndex]->Run(t_now);
		t_now_2 = GetElapsedMicros();
		
		Rte.SetSystemTime(t_now);
		
		runtimeRecord = t_now_2 - t_now;
		
		while (t_now_2 - t_now < timeSlot[taskIndex])
			t_now_2 = GetElapsedMicros();
		
		taskIndex++;
		if (taskIndex >= TASKPOOL_SIZE)
			taskIndex = 0;
	}
}


void HardFault_Handler(void)
{
	PORT->Group[0].OUTSET.reg = PORT_PA28;
	while (1)
	{
	}
}