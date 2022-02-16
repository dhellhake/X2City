/* 
* ComHandler.h
*
* Created: 16.01.2022 13:04:30
* Author: dominik hellhake
*/
#ifndef __COMHANDLER_H__
#define __COMHANDLER_H__

#include "sam.h"
#include "..\Task.h"

#define COMPILER_ALIGNED(a)        __attribute__((__aligned__(a)))

class ComHandler : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		uint8_t once = 0x00;
		uint8_t can_motorcontrol_message_buffer[64];
		uint8_t can_batterycontrol_message_buffer[64];
}; //ComHandler

extern ComHandler ComHdl;

#endif //__COMHANDLER_H__
