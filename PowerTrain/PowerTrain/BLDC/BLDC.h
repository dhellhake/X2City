/* 
* BLDC.h
*
* Created: 16.05.2022 21:59:20
* Author: dominik hellhake
*/
#ifndef __BLDC_H__
#define __BLDC_H__

#include "..\Task.h"
#include "..\DeviceDriver\TCC\TCClib.h"
#include "..\BLDC\BLDCPattern.h"
#include "..\HallSensor\Hall.h"

class BLDC : public Task
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
		float Tar_Duty = 2.0f;
		float Max_Duty = 9.0f;
	
		uint8_t once = 0;
		inline void Drive_SetPhase(HALL_STATE state)
		{				
			TCC0_SetDuty(this->Tar_Duty);
			
			uint32_t pattern = GetPattern(state, DrvDir_Forward);						
			TCC0_SetPattern(pattern);			
		}
	
}; //BLDC

extern BLDC DRV;

#endif //__BLDC_H__
