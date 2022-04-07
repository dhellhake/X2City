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
	
	#define DMA_CHID_COM	0
	
	void InitDMAC();
	
	inline void DMAC_ChannelStartTransfer(uint8_t chId)
	{
		DMAC->CHID.reg =		DMAC_CHID_ID(chId);
		DMAC->CHCTRLA.reg |=	DMAC_CHCTRLA_ENABLE;
	}
		
	void DMAC_ChannelSetSource(uint32_t* data, uint16_t len);	
	void DMAC_SetDescriptorSrcAddr(uint8_t descrId, uint32_t* addr);

#ifdef __cplusplus
}
#endif

#endif //__DMACLIB_H__
