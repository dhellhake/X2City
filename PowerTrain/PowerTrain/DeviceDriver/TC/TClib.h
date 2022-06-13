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
	void InitTC1();
	void InitTC2();
	void InitTC3();
	
	inline void TC3_SetEnabled(uint8_t enabled)
	{
		if (enabled > 0)
		{
			if ((TC3->COUNT16.CTRLA.reg & TC_CTRLA_ENABLE) == 0)
				TC3->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;
		} else
		{
			TC3->COUNT16.CTRLA.reg &= ~TC_CTRLA_ENABLE;
			TC3->COUNT16.COUNT.reg = 0;
		}
	}
	
	inline void TC3_SetInterval_us(uint16_t micros)
	{		
		TC3->COUNT16.CCBUF[0].reg = micros;
	}
	
#ifdef __cplusplus
}
#endif

#endif //__TCLIB_H__
