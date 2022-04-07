/* 
* EIClib.h
*
* Created: 28.08.2021 17:54:15
* Author: dominik hellhake
*/
#ifndef __EICLIB_H__
#define __EICLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "samc21.h"
	
	void InitEIC();	
		
	void EICEnableInterrupt(uint16_t extInt);
	void EICDisableInterrupt(uint16_t extInt);
	
	
#ifdef __cplusplus
}
#endif

#endif //__EICLIB_H__
