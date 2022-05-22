/*
* TCClib.h
*
* Created: 03.09.2021 12:30:17
* Author: dominik hellhake
*/
#ifndef __TCCLIB_H__
#define __TCCLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "samc21.h"
	
	#define TCC0_PER_VAL				2399		// f_PWM = 48Mhz / 1 (2399 + 1)  => 20Khz

	void InitTCC0();

	inline void TCC0_SetDuty(float duty)
	{
		TCC0->CC[0].reg	=	(TCC0_PER_VAL / 100) * duty;
	}

	inline void TCC0_SetPattern(uint32_t pattern)
	{
		TCC0->PATT.reg = pattern;
	}

#ifdef __cplusplus
}
#endif

#endif //__TCCLIB_H__