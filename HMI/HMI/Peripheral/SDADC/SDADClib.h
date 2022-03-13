/* 
* SDADClib.h
*
* Created: 17.02.2022 20:44:53
* Author: dominik hellhake
*/
#ifndef __SDADCLIB_H__
#define __SDADCLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "samc21.h"
	
	#define SDAC_VREF	1.024f
	
	void InitSDADC();
	
#ifdef __cplusplus
}
#endif

#endif //__SDADCLIB_H__
