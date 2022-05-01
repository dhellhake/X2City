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
	
	void InitSERCOM0();
	
	void SERCOM0_SetBAUD(uint32_t fbaud);
	
	inline void SERCOM0_RxEnable()
	{		
		SERCOM0->SPI.CTRLB.reg |= SERCOM_SPI_CTRLB_RXEN;
	}
	
	inline void SERCOM0_RxDisable()
	{		
		SERCOM0->SPI.CTRLB.reg &= ~(SERCOM_SPI_CTRLB_RXEN);
	}
	
	inline uint8_t SERCOM0_TransmitByte(uint8_t byte)
	{
		while(SERCOM0->SPI.INTFLAG.bit.DRE == 0);
		SERCOM0->SPI.DATA.reg = byte;
		while(SERCOM0->SPI.INTFLAG.bit.RXC == 0);
		
		return SERCOM0->SPI.DATA.reg;
	}
	
	#ifdef __cplusplus
}
#endif

#endif //__SERCOMLIB_H__
