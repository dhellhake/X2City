/* 
* SDADClib.h
*
* Created: 11.12.2022 15:38:34
* Author: dominik hellhake
*/

#ifndef __SDADCLIB_H__
#define __SDADCLIB_H__

#ifdef __cplusplus
	extern "C" {
#endif

	#include "samc21.h"
	
	extern float tmpVoltage;
	
	void InitSDADC();
	
#ifdef __cplusplus
	}
#endif

#endif //__SDADCLIB_H__
