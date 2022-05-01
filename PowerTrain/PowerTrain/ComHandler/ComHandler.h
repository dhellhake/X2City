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

typedef struct com_debug_record_t
{
	uint8_t EthHeader[16];
	uint32_t preamble;
	
	/* DRV8323 */
	uint32_t	Avl_DRV_State;
	float		Tar_Duty;
	
	/* HallSensor */
	uint32_t	Avl_AvgHallStateInterval;
	float		Avl_TicksPerSecond;
	uint32_t	Avl_Ticks;
	uint32_t	Avl_DriveDirection;
	uint32_t	Avl_HallState;

	uint32_t postamble;
} com_debug_record_t;

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
	
		com_debug_record_t *Record;
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
