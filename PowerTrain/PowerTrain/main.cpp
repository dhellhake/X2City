/*
 * PowerTrain.cpp
 *
 * Created: 01.05.2022 13:03:59
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "DeviceDriver/TC/TClib.h"
#include "ComHandler/ComHandler.h"
#include "BLDCDrive/BLDCDrive.h"
#include "HallSensor/HallSensor.h"

#define TASKPOOL_SIZE		1

Task* taskPool[TASKPOOL_SIZE] = {
	//&DRV,
	//&Hall,
	&ComHdl
};
uint16_t timeSlot[TASKPOOL_SIZE]
{
	//4,
	//4,
	10000
};

int main(void)
{
	uint64_t t_now = 0;
	uint64_t t_now_2 = 0;
	uint8_t taskIndex = 0;
	uint32_t runtimeRecord;
	
	while (1)
	{
		t_now = GetElapsedMicros();
		taskPool[taskIndex]->Run(t_now);
		t_now_2 = GetElapsedMicros();
		
		runtimeRecord = t_now_2 - t_now;
		
		while (t_now_2 - t_now < timeSlot[taskIndex])
			t_now_2 = GetElapsedMicros();
		
		taskIndex++;
		if (taskIndex >= TASKPOOL_SIZE)
			taskIndex = 0;
	}
}
