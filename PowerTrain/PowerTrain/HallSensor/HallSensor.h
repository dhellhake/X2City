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

enum class QU_AVG_HALL_INTERVAL
{
	INSTABLE = 0,
	CONTINUOUS = 1
};

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
		
		void HallTrigger(HALL_STATE newState, uint32_t interval);
		
		QU_AVG_HALL_INTERVAL	QuAvgHallStateInterval = QU_AVG_HALL_INTERVAL::INSTABLE;
		uint32_t				AvgHallStateInterval = 0;
	private:
		HALL_STATE				HallState = HALL_STATE::UNDEFINED_1;
				
		float HallStatIntervalAveragingFactor = 0.7f;
		
		uint32_t HallStateIntervalHistory[64];
		uint8_t HallStateIntervalHistoryIdx = 0;
		inline float GetHallStateInterval_RelativeStdDeriv()
		{	
			float avgHallInterval = 0.0f;
			for (uint8_t x = 0; x < 64; x++)
				avgHallInterval += HallStateIntervalHistory[x];
			avgHallInterval /= 64;
			
			float variance = 0.0f;
			for (uint8_t x = 0; x < STATE_INTERVAL_HISTORY_SIZE; x++)
				variance += pow(this->HallStateIntervalHistory[x] - avgHallInterval, 2);
			variance = variance/(STATE_INTERVAL_HISTORY_SIZE - 1);
			
			return sqrt(variance) / (avgHallInterval / 100.0f);
		}
		
}; //HallSensor

extern HallSensor Hall;

#endif //__HALLSENSOR_H__
