/* 
* KSZ8851.h
*
* Created: 04.04.2022 20:11:12
* Author: dominik hellhake
*/
#ifndef __KSZ8851_H__
#define __KSZ8851_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "samc21.h"
#include "KSZ8851lib.h"
		
	uint8_t KSZ8851_Init();
	void KSZ8851_Handler();

	uint16_t KSZ8851_ReadRegister(uint8_t addr);

	void KSZ8851_WriteRegister(uint8_t addr, uint16_t data);
	uint8_t KSZ8851_WriteRegisterAndCheck(uint8_t addr, uint16_t data, uint16_t expected);
	void KSZ8851_SetRegisterBit(uint8_t addr, uint16_t mask);
	void KSZ8851_ClearRegisterBit(uint8_t addr, uint16_t mask);

	uint8_t KSZ8851_SendPacketDMAC();
	void KSZ8851_SendPacketFinalize();
	
	uint16_t KSZ8851_ReceivePacket();

	extern uint8_t TxPacketBuffer[KSZ8851_TX_BUFFER_SIZE];
	extern uint8_t RxPacketBuffer[KSZ8851_RX_BUFFER_SIZE];

#ifdef __cplusplus
}
#endif


#endif //__KSZ8851_H__
