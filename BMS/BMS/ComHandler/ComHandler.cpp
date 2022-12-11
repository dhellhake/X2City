/* 
* ComHandler.cpp
*
* Created: 16.01.2022 13:04:30
* Author: dominik hellhake
*/
#include "ComHandler.h"
#include "..\Peripheral\CAN\CANlib.h"


#define CAN_FILTER_ID_MOTORCONTROL         0x45a
#define CAN_FILTER_ID_BATTERYCONTROL        0x25a

ComHandler ComHdl;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ComHandler::Run(uint32_t timeStamp)
{
	can0_com();
		
	uint8_t data[64] = { 0x00 };
		
	for (uint8_t x = 0; x < 4; x++)
		data[x] = ((uint8_t*)((float*)&this->TxBuffer.IBat3))[x];
	
	can0_transmit(CAN_FILTER_ID_MOTORCONTROL, 64, data);
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
