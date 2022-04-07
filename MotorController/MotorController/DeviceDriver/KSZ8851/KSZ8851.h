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

	uint16_t ReadRegister(uint8_t addr);

	void WriteRegister(uint8_t addr, uint16_t data);
	uint8_t WriteRegisterAndCheck(uint8_t addr, uint16_t data, uint16_t expected);
	void SetRegisterBit(uint8_t addr, uint16_t mask);
	void ClearRegisterBit(uint8_t addr, uint16_t mask);

	uint8_t SendPacketDMAC();
	void SendPacketFinalize();


	extern uint8_t TxPacketBuffer[KSZ8851_TX_BUFFER_SIZE];

#ifdef __cplusplus
}
#endif


#endif //__KSZ8851_H__
