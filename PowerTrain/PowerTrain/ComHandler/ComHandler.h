/* 
* ComHandler.h
*
* Created: 15.03.2022 20:07:15
* Author: dominik hellhake
*/
#ifndef __COMHANDLER_H__
#define __COMHANDLER_H__

#include "..\Task.h"

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
}; //ComHandler

extern ComHandler ComHdl;

#endif //__COMHANDLER_H__
