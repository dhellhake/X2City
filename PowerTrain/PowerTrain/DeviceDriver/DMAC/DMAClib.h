/* 
* DMAClib.h
*
* Created: 04.09.2021 17:43:00
* Author: dominik hellhake
*/
#ifndef __DMACLIB_H__
#define __DMACLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "sam.h"
	
	#define DMA_CHID_BLDC_U		0
	#define DMA_CHID_BLDC_V		1
	#define DMA_CHID_BLDC_W		2
	
	void InitDMAC();
	
	inline void DMAC_ChannelForceStartTransfer(uint8_t chId)
	{
		DMAC->CHID.reg =		DMAC_CHID_ID(chId);
		DMAC->CHCTRLA.reg &=	~DMAC_CHCTRLA_ENABLE;
		while ((DMAC->CHCTRLA.reg & DMAC_CHCTRLA_ENABLE) != 0);
		DMAC->CHCTRLA.reg |=	DMAC_CHCTRLA_ENABLE;
	}

#ifdef __cplusplus
}
#endif

#endif //__DMACLIB_H__
