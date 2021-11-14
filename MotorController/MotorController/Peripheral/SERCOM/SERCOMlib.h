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
	void InitSERCOM5();
	
	inline void SERCOM5_SendBytes(uint16_t cnt, uint8_t* data)
	{
		for (uint16_t x = 0; x < cnt; x++)
		{
			while (!(SERCOM5->USART.INTFLAG.reg & SERCOM_USART_INTFLAG_DRE));
			SERCOM5->USART.DATA.reg = data[x];
		}
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
