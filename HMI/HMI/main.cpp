/*
 * HMI.cpp
 *
 * Created: 07.01.2022 14:54:57
 * Author : dominik hellhake
 */ 
#include "sam.h"
#include "Peripheral/CortexM0/CortexM0.h"
#include "SSD1327/SSD1327.h"
#include "Test/Test.h"

#define TASKPOOL_SIZE	1

Task* taskPool[TASKPOOL_SIZE] = {
	&OLED
};
uint16_t timeSlot[TASKPOOL_SIZE]
{
	10
};

typedef enum RecordType : uint16_t
{
	Runtime =			0xEE,
} RecordType;

typedef struct RuntimeRecord
{
	uint32_t Runtime[TASKPOOL_SIZE] { 0 };
	uint64_t ElapsedMicros	= 0;
	RecordType Type			= RecordType::Runtime;
	uint16_t Postamble		= 0xAA55;
} RuntimeRecord;

int main(void)
{	
	uint64_t t_now = 0;
	uint64_t t_now_2 = 0;
	uint8_t taskIndex = 0;
	RuntimeRecord runtimeRecord;
	
	while (1)
	{
		t_now = ElapsedMilis;
		taskPool[taskIndex]->Run(ElapsedMilis);
		t_now_2 = ElapsedMilis;
		
		runtimeRecord.Runtime[taskIndex] = t_now_2 - t_now;
		
		while (t_now_2 - t_now < timeSlot[taskIndex])
			t_now_2 = ElapsedMilis;
		
		taskIndex++;
		if (taskIndex >= TASKPOOL_SIZE)
			taskIndex = 0;
	}
}