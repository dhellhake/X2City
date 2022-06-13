/* 
* HallSensor.cpp
*
* Created: 14.11.2021 12:39:47
* Author: dominik hellhake
*/
#include "HallSensor.h"
#include "..\BLDC\BLDC.h"
#include "..\DeviceDriver\TC\TClib.h"
#include "..\DeviceDriver\DMAC\DMAClib.h"
#include "..\RuntimeEnvironment\RuntimeEnvironment.h"

HallSensor Hall;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT HallSensor::Run(uint32_t timeStamp)
{	
	float stdDeriv = this->GetHallStateInterval_RelativeStdDeriv();
	
	if (this->AvgHallStateInterval > 0)
		this->QuAvgHallStateInterval = stdDeriv < 30.0f ? QU_AVG_HALL_INTERVAL::CONTINUOUS : QU_AVG_HALL_INTERVAL::INSTABLE;
	else
		this->QuAvgHallStateInterval = QU_AVG_HALL_INTERVAL::INSTABLE;
				
	return RUN_RESULT::SUCCESS;
}


/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
void HallSensor::HallTrigger(HALL_STATE newState, uint32_t interval)
{	
	if (this->HallState == newState)
		return;
		
	if (this->QuAvgHallStateInterval == QU_AVG_HALL_INTERVAL::CONTINUOUS)
	{
		TC3_SetInterval_us((this->AvgHallStateInterval * 4) / 120);
		DRV.Drive_SetPhase(newState, COMMUTATION_TYPE::SINUSOIDAL);	
		
		switch(newState)
		{
			case HALL_STATE::HALL_STATE_4:
				DMAC_ChannelForceStartTransfer(DMA_CHID_BLDC_W);
			break;
			case HALL_STATE::HALL_STATE_6:
				DMAC_ChannelForceStartTransfer(DMA_CHID_BLDC_V);
			break;
			case HALL_STATE::HALL_STATE_2:
				DMAC_ChannelForceStartTransfer(DMA_CHID_BLDC_U);
			break;
			default:
			break;
		}
		
		TC3_SetEnabled(1);
	} else
	{
		TC3_SetEnabled(0);
		DRV.Drive_SetPhase(newState, COMMUTATION_TYPE::TRAPEZOIDAL);
	}		
	
	this->AvgHallStateInterval = (HallStatIntervalAveragingFactor * (float)interval) + ((1.0f - HallStatIntervalAveragingFactor) * (float)HallStateIntervalHistory[HallStateIntervalHistoryIdx]);
	
	HallStateIntervalHistory[HallStateIntervalHistoryIdx++] = interval;
	if (HallStateIntervalHistoryIdx >= 64)
		HallStateIntervalHistoryIdx = 0;
		
	this->HallState = newState;
	Rte.Record.Avl_Ticks++;
}