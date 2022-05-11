/* 
* ComHandler.h
*
* Created: 15.03.2022 20:07:15
* Author: dominik hellhake
*/
#ifndef __COMHANDLER_H__
#define __COMHANDLER_H__

#include "..\Task.h"
#include "..\DeviceDriver\KSZ8851\KSZ8851.h"

typedef enum DEBUG_LINK_STATE : uint8_t
{
	DEBUG_STATE_OFFLINE =			0x00,
	DEBUG_STATE_READY =				0x01,
	DEBUG_STATE_TX_PENDING =		0x02,
} DEBUG_LINK_STATE;


class ComHandler : public Task
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
		volatile DEBUG_LINK_STATE DebugLinkState =		DEBUG_STATE_OFFLINE;
		volatile uint8_t EthRxPacketCount =				0x00;
		
		bool once = false;

		inline void SetDebugLinkState(uint8_t state)
		{
			if (state > 0x00)
				this->DebugLinkState = DEBUG_STATE_READY;
			else
				this->DebugLinkState = DEBUG_STATE_OFFLINE;
		}

}; //ComHandler

extern ComHandler ComHdl;

#endif //__COMHANDLER_H__
