/* 
* CANlib.h
*
* Created: 16.01.2022 10:02:22
* Author: dominik hellhake
*/
#ifndef __CANLIB_H__
#define __CANLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include <stdalign.h>
	#include "samc21.h"


	void InitCAN0();

	void can0_transmit(uint8_t size, uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif //__CANLIB_H__
