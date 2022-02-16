/*
 * HMI.cpp
 *
 * Created: 07.01.2022 14:54:57
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "Peripheral/CortexM0/CortexM0.h"
#include "ComHandler/ComHandler.h"
#include "SSD1327/SSD1327.h"


#define TASKPOOL_SIZE	2

Task* taskPool[TASKPOOL_SIZE] = {
	&ComHdl,
	&OLED
};
uint16_t timeSlot[TASKPOOL_SIZE]
{
	10,
	10
};

int main(void)
{	
	uint64_t t_now = 0;
	uint64_t t_now_2 = 0;
	uint8_t taskIndex = 0;
	uint32_t runtimeRecord;
	
	while (1)
	{
		t_now = ElapsedMilis;
		taskPool[taskIndex]->Run(ElapsedMilis);
		t_now_2 = ElapsedMilis;
		
		runtimeRecord = t_now_2 - t_now;
		
		while (t_now_2 - t_now < timeSlot[taskIndex])
			t_now_2 = ElapsedMilis;
		
		taskIndex++;
		if (taskIndex >= TASKPOOL_SIZE)
			taskIndex = 0;
	}
}