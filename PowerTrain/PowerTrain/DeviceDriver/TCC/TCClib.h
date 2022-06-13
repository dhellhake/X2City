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
	
	#define TCC0_PER_VAL				1200		// f_PWM = 48Mhz / 1 (2399 + 1)  => 20Khz
	#define TCC0_CC_U_IDX				3
	#define TCC0_CC_V_IDX				2
	#define TCC0_CC_W_IDX				0

	void InitTCC0();

	inline void TCC0_SetDuty(float duty, uint8_t cc)
	{
		TCC0->CCBUF[cc].reg	=	(uint32_t)(((float)(TCC0_PER_VAL / 100)) * duty);
	}
	
	inline void TCC0_SetDutyRaw(uint32_t ccVal, uint8_t cc)
	{
		TCC0->CCBUF[cc].reg	=	ccVal;
	}

	inline void TCC0_SetPattern(uint32_t pattern)
	{
		TCC0->PATTBUF.reg = pattern;
	}

#ifdef __cplusplus
}
#endif

#endif //__TCCLIB_H__