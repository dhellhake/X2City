/* 
* ComHandler.cpp
*
* Created: 15.03.2022 20:07:15
* Author: dominik hellhake
*/
#include "ComHandler.h"
#include "..\DeviceDriver\DMAC\DMAClib.h"
#include "..\DeviceDriver\KSZ8851\KSZ8851.h"

ComHandler ComHdl;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ComHandler::Run(uint32_t timeStamp)
{		
	if (this->DebugState == DEBUG_STATE_OFFLINE)
		return RUN_RESULT::ERROR;
	
	if (!this->once)
	{		
		for (uint16_t x = 16; x < 1516; x++)
			TxPacketBuffer[x] = 0xDE;
			
		
		this->Record = (com_debug_record_t *)TxPacketBuffer;
		this->Record->preamble = 0x11111111;
		this->Record->postamble = 0x33333333;
		this->Record->Avl_HallState = 0x22222222;
		
		SendPacketDMAC();
		
		this->once = true;
	}
	
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
