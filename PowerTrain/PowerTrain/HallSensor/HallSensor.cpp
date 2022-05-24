/* 
* HallSensor.cpp
*
* Created: 14.11.2021 12:39:47
* Author: dominik hellhake
*/
#include "HallSensor.h"
#include "..\BLDC\BLDC.h"
#include "..\RuntimeEnvironment\RuntimeEnvironment.h"
#include "..\DeviceDriver\CortexM0\CortexM0.h"

HallSensor Hall;

HallSensor::HallSensor()
{
}

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
bool once = false;
RUN_RESULT HallSensor::Run(uint32_t timeStamp)
{
	if (!once)
	{
		once = true;
		
		HALL_STATE newState = (HALL_STATE)((PORT->Group[0].IN.reg >> 23) & 0b111);
		DRV.Drive_SetPhase(newState);
	}
	
	uint16_t t1 = SysTick->VAL;
	float stdAbw = this->GetHallStateInterval_RelativeStdDeriv();
	if (stdAbw < 20.0f)
		PORT->Group[0].OUTSET.reg = PORT_PA13;
	else	
		PORT->Group[0].OUTCLR.reg = PORT_PA13;
	
	t1 -= SysTick->VAL;
			
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
uint32_t HallStateLastTstmp = 0;
void HallSensor::HallTrigger(HALL_STATE newState, uint32_t tstmp_micros)
{	
	if (this->HallState == newState)
		return;
		
	DRV.Drive_SetPhase(newState);
		
	/* Add current tick to history */
	this->HallStateIntervalHistory[this->HallStateIntervalHistoryIdx++] = tstmp_micros - HallStateLastTstmp;
	HallStateLastTstmp = tstmp_micros;
	if (this->HallStateIntervalHistoryIdx >= STATE_INTERVAL_HISTORY_SIZE)
		this->HallStateIntervalHistoryIdx = 0;
	
	this->HallState = newState;
	Rte.Record.Avl_Ticks++;		
}