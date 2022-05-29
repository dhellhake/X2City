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

	void InitTC3();
	void InitTC0_1();
	
	// Overflow of 32bit-TC2/3 running at 1Mhz
	extern volatile uint32_t TC0_Overflow;
	inline uint64_t GetElapsedMicros()
	{	
		TC0->COUNT32.CTRLBSET.reg = TC_CTRLBSET_CMD_READSYNC;		
		while (TC0->COUNT32.SYNCBUSY.bit.COUNT);
		return (TC0_Overflow * 0xFFFFFFFF) + (TC0->COUNT32.COUNT.reg);
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
