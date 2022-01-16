/* 
* ComHandler.cpp
*
* Created: 16.01.2022 13:04:30
* Author: dominik hellhake
*/
#include "ComHandler.h"
#include "..\Peripheral\CAN\CANlib.h"

ComHandler ComHdl;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ComHandler::Run(uint32_t timeStamp)
{
	can0_com();
	can1_com();
	
	if (once > 0)
	{
		once = 0;
		
		uint8_t data[64] = { 0x00 };
		for (uint8_t x = 0; x < 64; x++)
			data[x] = x;
		
		can0_transmit(64, data);		
	} else 
	{
		once = 0;
	}
	
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
