/* 
* BLDC.cpp
*
* Created: 16.05.2022 21:59:19
* Author: dominik hellhake
*/
#include "BLDC.h"
#include "..\HallSensor\HallSensor.h"

BLDC DRV;

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
RUN_RESULT BLDC::Run(uint32_t timeStamp)
{
	if (Hall.HallStateIntervalHistoryIdx != 0)
		if (this->Max_Duty > this->Tar_Duty)
		{			
			this->Tar_Duty += 0.05f;
			for (uint8_t x = 0; x < 120; x++)
				this->sineTarDuty[x] = this->Tar_Duty * sine120[x];
		}
	
	return RUN_RESULT::SUCCESS;
}