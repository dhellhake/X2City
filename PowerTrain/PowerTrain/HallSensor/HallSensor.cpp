/* 
* HallSensor.cpp
*
* Created: 14.11.2021 12:39:47
* Author: dominik hellhake
*/
#include "HallSensor.h"
#include "..\RuntimeEnvironment\RuntimeEnvironment.h"
#include "..\DeviceDriver\CortexM0\CortexM0.h"

HallSensor Hall;

HallSensor::HallSensor()
{
	Rte.SetHallState((HALL_STATE)((PORT->Group[0].IN.reg >> 23) & 0b111));
}

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT HallSensor::Run(uint32_t timeStamp)
{		
	Rte.SetHallStateInterval(this->GetAvgHallStateInterval());
	Rte.SetHallTicksPerSecond(1.0f / (((float)Rte.GetHallStateInterval())) * 1000000.0f);
	
	if (this->LastHallStateSwitchTime + 200000 < timeStamp)
	{
		Rte.SetHallTicksPerSecond(0);
		Rte.SetHallStateInterval(0);
	}
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
void HallSensor::HallTrigger(HallSignal source, uint32_t tstmp_micros)
{
	/* Update average hall state transition interval */
	this->AvgHallStateInvervalHistory[this->AvgHallStateIntervalHistoryIdx++] = tstmp_micros - this->LastHallStateSwitchTime;
	if (this->AvgHallStateIntervalHistoryIdx >= STATE_INTERVAL_HISTORY_SIZE)
		this->AvgHallStateIntervalHistoryIdx = 0;
	
	/* Add current tick to history */
	this->LastHallStateSwitchTime = tstmp_micros;
	this->HallStateInvervalHistory[this->HallStateIntervalHistoryIdx++] = tstmp_micros;
	if (this->HallStateIntervalHistoryIdx >= STATE_INTERVAL_HISTORY_SIZE)
		this->HallStateIntervalHistoryIdx = 0;
	
	Rte.SetHallState((HALL_STATE)((PORT->Group[0].IN.reg >> 23) & 0b111));
	Rte.SetHallTicks(Rte.GetHallTicks() + 1);
}