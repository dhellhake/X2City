/* 
* TClib.h
*
* Created: 05.09.2021 12:04:54
* Author: dominik hellhake
*/
#ifndef __TCLIB_H__
#define __TCLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "sam.h"

	void InitTC0();
	void InitTC2_3();
	
	// Overflow of 32bit-TC2/3 running at 1Mhz
	extern volatile uint32_t TC2_Overflow;
	inline uint64_t GetElapsedMicros()
	{
		TC2->COUNT32.CTRLBSET.reg = TC_CTRLBSET_CMD_READSYNC;
		while (TC2->COUNT32.SYNCBUSY.bit.COUNT);
		return (TC2_Overflow * 0xFFFFFFFF) + (TC2->COUNT32.COUNT.reg) + (uint64_t)((uint64_t)TC2_Overflow << (uint64_t)32) ;
	}
	
	inline void Delay_ms(uint16_t milis)
	{
		uint32_t tarms = (GetElapsedMicros() / 1000) + milis;
		while ((GetElapsedMicros() / 1000) < tarms);
	}

	
#ifdef __cplusplus
}
#endif

#endif //__TCLIB_H__
