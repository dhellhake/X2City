/* 
* BLDC.cpp
*
* Created: 16.05.2022 21:59:19
* Author: dominik hellhake
*/
#include "BLDC.h"

BLDC DRV;

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
RUN_RESULT BLDC::Run(uint32_t timeStamp)
{
	if (this->Max_Duty > this->Tar_Duty)
		this->Tar_Duty += 0.1f;
	
	return RUN_RESULT::SUCCESS;
}