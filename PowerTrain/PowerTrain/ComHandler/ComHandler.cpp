/* 
* ComHandler.cpp
*
* Created: 15.03.2022 20:07:15
* Author: dominik hellhake
*/
#include "ComHandler.h"
#include "..\DeviceDriver\DMAC\DMAClib.h"
#include "..\DeviceDriver\KSZ8851\KSZ8851.h"
#include "..\DeviceDriver\TC\TClib.h"

ComHandler ComHdl;

uint32_t tme[100];
uint8_t tmeIdx = 0x00;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ComHandler::Run(uint32_t timeStamp)
{		
	if (this->DebugLinkState == DEBUG_STATE_OFFLINE)
		return RUN_RESULT::ERROR;
	
	if (!this->once)
	{
		
		this->once = true;
	}
		
	KSZ8851_SendPacketDMAC();
	
	if (this->DebugLinkState != DEBUG_STATE_TX_PENDING && this->EthRxPacketCount > 0)
	{	
		uint16_t btCnt = KSZ8851_ReceivePacket();
		
		
		tme[tmeIdx++] = 0;
		
		if (tmeIdx >= 10)
			tmeIdx = 0;
	}
	
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
