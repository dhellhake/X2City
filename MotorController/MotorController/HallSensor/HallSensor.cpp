/* 
* HallSensor.cpp
*
* Created: 14.11.2021 12:39:47
* Author: dominik hellhake
*/
#include "HallSensor.h"
#include "..\DeviceDriver\CortexM0\CortexM0.h"

HallSensor Hall;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT HallSensor::Run(uint32_t timeStamp)
{
	this->Avl_AvgHallStateInterval = this->GetAvgHallStateInterval();
	this->Avl_TicksPerSecond = 1.0f / (((float)this->Avl_AvgHallStateInterval) / 1000000.0f);
	
	if ((this->LastHallStateSwitchTime_ms / 1000) + 200 < timeStamp)
	{
		this->Avl_TicksPerSecond = 0;
		this->Avl_AvgHallStateInterval = 0;
	}
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
void HallSensor::HallTrigger(HallSignal source, uint32_t tstmp)
{
	/* Update average hall state transition interval */
	this->AvgHallStateInvervalHistory[this->AvgHallStateIntervalHistoryIdx++] = tstmp - this->LastHallStateSwitchTime_ms;
	if (this->AvgHallStateIntervalHistoryIdx >= STATE_INTERVAL_HISTORY_SIZE)
		this->AvgHallStateIntervalHistoryIdx = 0;
	
	/* Add current tick to history */
	this->LastHallStateSwitchTime_ms = tstmp;
	this->HallStateInvervalHistory[this->HallStateIntervalHistoryIdx++] = tstmp;
	if (this->HallStateIntervalHistoryIdx >= STATE_INTERVAL_HISTORY_SIZE)
		this->HallStateIntervalHistoryIdx = 0;
	
	this->Avl_HallState = (HALL_STATE)((PORT->Group[0].IN.reg >> 23) & 0b111);
	this->Avl_TriggerCnt++;
}