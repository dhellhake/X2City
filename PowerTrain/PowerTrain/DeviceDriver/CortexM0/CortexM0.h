/*
* CortexM0.h
*
* Created: 14.11.2021 09:06:25
*  Author: Dominik Hellhake
*/
#ifndef __CORTEXM0_H__
#define __CORTEXM0_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "samc21.h"

	void InitSysTick();

	// Overflow of 32bit-TC2/3 running at 1Mhz
	extern volatile uint64_t SysTick_Overflow;
	inline uint64_t GetElapsedMicros()
	{
		return ((SysTick_Overflow << 24) + ((uint64_t)(0xFFFFFF - SysTick->VAL))) / (uint64_t)48;
	}
	
#ifdef __cplusplus
}
#endif

#endif //__CORTEXM0_H__
