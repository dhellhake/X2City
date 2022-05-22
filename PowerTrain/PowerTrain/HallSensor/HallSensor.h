/* 
* HallSensor.h
*
* Created: 14.11.2021 12:39:47
* Author: dominik hellhake
*/
#ifndef __HALLSENSOR_H__
#define __HALLSENSOR_H__

#include "..\Task.h"
#include "Hall.h"

#define STATE_INTERVAL_HISTORY_SIZE		512

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
	
	private:
		HALL_STATE		HallState = HALL_STATE::UNDEFINED_1;
		
		uint32_t		HallStateInvervalHistory[STATE_INTERVAL_HISTORY_SIZE] = { 0x00 };			
						
		/* Average Time between Hall State Changes */
		volatile inline uint32_t GetAvgHallStateInterval()
		{
			uint32_t result = 0;
			for (uint16_t x = 0; x < STATE_INTERVAL_HISTORY_SIZE; x++)
				result += this->HallStateInvervalHistory[x];
			return result / STATE_INTERVAL_HISTORY_SIZE;
		}
	
}; //HallSensor

extern HallSensor Hall;

#endif //__HALLSENSOR_H__
