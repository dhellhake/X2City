/* 
* BLDCDrive.cpp
*
* Created: 14.11.2021 12:04:17
* Author: dominik hellhake
*/
#include "BLDCDrive.h"
#include "..\Peripheral\CortexM0\CortexM0.h"

BLDCDrive DRV;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT BLDCDrive::Run(uint32_t timeStamp)
{	
	return RUN_RESULT::SUCCESS;
}