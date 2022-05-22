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
RUN_RESULT HallSensor::Run(uint32_t timeStamp)
{		
	if (this->HallStateInvervalHistory[this->HallStateIntervalHistoryIdx] + 200000 < timeStamp)
		Rte.Record.Avl_TicksPerSecond = 0.0f;
	else
		Rte.Record.Avl_TicksPerSecond = 1.0f / (((float)this->GetAvgHallStateInterval())) * 1000000.0f;
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
void HallSensor::HallTrigger(HALL_STATE newState, uint32_t tstmp_micros)
{	
	if (this->HallState == newState)
		return;
	
	/* Add current tick to history */
	this->HallStateInvervalHistory[this->HallStateIntervalHistoryIdx++] = tstmp_micros;
	if (this->HallStateIntervalHistoryIdx >= STATE_INTERVAL_HISTORY_SIZE)
		this->HallStateIntervalHistoryIdx = 0;
	
	this->HallState = newState;
	Rte.Record.Avl_Ticks++;
		
	DRV.Drive_SetPhase(newState);
}