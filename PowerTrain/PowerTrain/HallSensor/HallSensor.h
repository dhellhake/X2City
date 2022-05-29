/* 
* HallSensor.h
*
* Created: 14.11.2021 12:39:47
* Author: dominik hellhake
*/
#ifndef __HALLSENSOR_H__
#define __HALLSENSOR_H__

#include <math.h>
#include "..\Task.h"
#include "Hall.h"

#define STATE_INTERVAL_HISTORY_SIZE		64

class HallSensor : public Task
{
	/************************************************************************/
	/* Task Interface implementation										*/
	/************************************************************************/
	public:
		virtual RUN_RESULT Run(uint32_t timeStamp);
	
	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		HallSensor();
	
		void HallTrigger(HALL_STATE newState, uint32_t tstmp_micros);
		
		uint16_t		HallStateIntervalHistoryIdx = 0;
		HALL_STATE history[256];
		
		uint8_t AvlWheelSpeedStability = 0x00;
	private:
		HALL_STATE		HallState = HALL_STATE::UNDEFINED_1;
		
		inline float GetAverageHallStateInterval()
		{			
			float result = 0.0f;
			for (uint8_t x = 0; x < STATE_INTERVAL_HISTORY_SIZE; x++)
				result += this->HallStateIntervalHistory[x];
			return result / STATE_INTERVAL_HISTORY_SIZE;			
		}
		float avgHallStateInterval;
		inline float GetHallStateInterval_RelativeStdDeriv()
		{			
			avgHallStateInterval = this->GetAverageHallStateInterval();
			
			float variance = 0.0f;
			for (uint8_t x = 0; x < STATE_INTERVAL_HISTORY_SIZE; x++)
				variance += pow(this->HallStateIntervalHistory[x] - avgHallStateInterval, 2);
			variance = variance/(STATE_INTERVAL_HISTORY_SIZE - 1);
			
			return sqrt(variance) / (avgHallStateInterval / 100.0f);
		}
		
		uint32_t		HallStateIntervalHistory[STATE_INTERVAL_HISTORY_SIZE] = { 0x00 };				
}; //HallSensor

extern HallSensor Hall;

#endif //__HALLSENSOR_H__
