/* 
* HallSensor.cpp
*
* Created: 14.11.2021 12:39:47
* Author: dominik hellhake
*/
#include "HallSensor.h"
#include "..\Peripheral\CortexM0\CortexM0.h"

HallSensor Hall;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT HallSensor::Run(uint32_t timeStamp)
{
	PORT->Group[0].OUTTGL.reg = PORT_PA12;
	
	return RUN_RESULT::SUCCESS;
}
