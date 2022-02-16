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
	
}; //ComHandler

extern ComHandler ComHdl;

#endif //__COMHANDLER_H__
