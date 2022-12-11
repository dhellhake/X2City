/* 
* SERCOMlib.h
*
* Created: 20.09.2020 13:55:34
* Author: dominik hellhake
*/
#ifndef __SERCOMLIB_H__
#define __SERCOMLIB_H__

#ifdef __cplusplus
extern "C" {
	#endif

	#include "samc21.h"
	
	void InitSERCOM1();
	
	inline uint8_t SERCOM1_TransmitByte(uint8_t byte)
	{
		while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
		SERCOM1->SPI.DATA.reg = byte;
		while(SERCOM1->SPI.INTFLAG.bit.RXC == 0);
		
		return SERCOM1->SPI.DATA.reg;
	}
	
	#ifdef __cplusplus
}
#endif

#endif //__SERCOMLIB_H__
