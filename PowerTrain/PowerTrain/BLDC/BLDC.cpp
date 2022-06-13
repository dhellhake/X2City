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
	if (Hall.AvgHallStateInterval != 0)
		if (this->Max_Duty > this->Tar_Duty)
		{			
			this->Tar_Duty += 0.0005f;
			for (uint8_t x = 0; x < 120; x++)
				this->TarSineDuty[x] = (uint32_t)(this->Tar_Duty * sine120[x] * 1200.0f);
		}
	
	return RUN_RESULT::SUCCESS;
}