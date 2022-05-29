/* 
* HallSensor.cpp
*
* Created: 14.11.2021 12:39:47
* Author: dominik hellhake
*/
#include "HallSensor.h"
#include "..\BLDC\BLDC.h"
#include "..\RuntimeEnvironment\RuntimeEnvironment.h"
#include "..\DeviceDriver\TC\TClib.h"
#include "..\DeviceDriver\TCC\TCClib.h"

HallSensor Hall;

HallSensor::HallSensor()
{
}

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
bool once = false;
bool oncet = false;
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
		
	if (this->avgHallStateInterval <= 12000 && this->avgHallStateInterval >= 7000)
	{
		oncet = true;
		AvlWheelSpeedStability = 0x01;
		TC3->COUNT16.CTRLA.reg |= (1 << TC_CTRLA_ENABLE_Pos);
		TC3->COUNT16.CC[0].reg = (this->avgHallStateInterval * 4) / 120;
	}
	else	
	{
		if (oncet)
		{
			oncet = false;
		}
		
		AvlWheelSpeedStability = 0x00;
		TC3->COUNT16.CTRLA.reg &= ~(1 << TC_CTRLA_ENABLE_Pos);
	}
	
	t1 -= SysTick->VAL;
			
	return RUN_RESULT::SUCCESS;
}


/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
uint32_t HallStateLastTstmp = 0;
uint8_t PhaseUSineIdx;
uint8_t PhaseVSineIdx;
uint8_t PhaseWSineIdx;
uint16_t historyIdx = 0;
void HallSensor::HallTrigger(HALL_STATE newState, uint32_t tstmp_micros)
{	
	if (this->HallState == newState)
		return;		
	
	history[historyIdx++] = newState;
	if (historyIdx >= 256)
		historyIdx = 0;
	
	if (AvlWheelSpeedStability)
	{
		switch(newState)
		{
			case HALL_STATE::HALL_STATE_4:
				PhaseWSineIdx = 0;
				PhaseVSineIdx = 120;
				TCC0_SetDuty(0.0f, TCC0_CC_W_IDX);
				TCC0_SetDuty(0.0f, TCC0_CC_V_IDX);
				//TC3->COUNT16.COUNT.reg = TC3->COUNT16.CC[0].reg;
			break;
			case HALL_STATE::HALL_STATE_6:
				PhaseVSineIdx = 0;
				PhaseUSineIdx = 120;
				TCC0_SetDuty(0.0f, TCC0_CC_V_IDX);
				TCC0_SetDuty(0.0f, TCC0_CC_U_IDX);
				//TC3->COUNT16.COUNT.reg = TC3->COUNT16.CC[0].reg;
			break;
			case HALL_STATE::HALL_STATE_2:
				PhaseUSineIdx = 0;
				PhaseWSineIdx = 120;
				TCC0_SetDuty(0.0f, TCC0_CC_U_IDX);
				TCC0_SetDuty(0.0f, TCC0_CC_W_IDX);
				//TC3->COUNT16.COUNT.reg = TC3->COUNT16.CC[0].reg;
			break;
			default:
			break;
		}
	}	
		
	PORT->Group[0].OUTTGL.reg = PORT_PA13;
	DRV.Drive_SetPhase(newState);
	
	
	/* Add current tick to history */
	this->HallStateIntervalHistory[this->HallStateIntervalHistoryIdx++] = tstmp_micros - HallStateLastTstmp;
	HallStateLastTstmp = tstmp_micros;
	if (this->HallStateIntervalHistoryIdx >= STATE_INTERVAL_HISTORY_SIZE)
		this->HallStateIntervalHistoryIdx = 0;
	
	this->HallState = newState;
	Rte.Record.Avl_Ticks++;
	
}



void TC3_Handler(void)
{	
	PORT->Group[0].OUTTGL.reg = PORT_PA11;
	
	if (PhaseUSineIdx < 120)
		TCC0_SetDuty(DRV.sineTarDuty[PhaseUSineIdx++], TCC0_CC_U_IDX);
		
	if (PhaseVSineIdx < 120)
		TCC0_SetDuty(DRV.sineTarDuty[PhaseVSineIdx++], TCC0_CC_V_IDX);
		
	if (PhaseWSineIdx < 120)
		TCC0_SetDuty(DRV.sineTarDuty[PhaseWSineIdx++], TCC0_CC_W_IDX);
	
	TC3->COUNT16.INTFLAG.reg = TC_INTFLAG_OVF;
}