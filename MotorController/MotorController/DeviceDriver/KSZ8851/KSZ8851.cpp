/* 
* KSZ8851.cpp
*
* Created: 04.04.2022 20:11:12
* Author: dominik hellhake
*/
#include "KSZ8851.h"
#include "..\TC\TClib.h"
#include "..\SERCOM\SERCOMlib.h"
#include "..\EIC\EIClib.h"
#include "..\DMAC\DMAClib.h"
#include "..\..\ComHandler\ComHandler.h"

void KSZ8851_ReadFifo(uint8_t *data, uint16_t cnt);

uint8_t RxPacketBuffer[KSZ8851_RX_BUFFER_SIZE];
uint8_t TxPacketBuffer[KSZ8851_TX_BUFFER_SIZE] = 
{
	0x00, 0x00,
	0xEC, 0x05,								// Packet Size
	0x04, 0xD4, 0xC4, 0xE4, 0x63, 0xCA,		// Destination Address
	0x00 ,0x04, 0x25, 0x19, 0x23, 0x9B		// Source Address
};

void KSZ8851_Handler()
{
	uint16_t isr = KSZ8851_ReadRegister(KSZ8851_ISR);
	
	// Release Interrupt Line
	KSZ8851_WriteRegister(KSZ8851_IER, 0x0000);
	
	// Link Status Changed
	if ((isr & KSZ8851_ISR_LCIS) != 0)
	{
		KSZ8851_WriteRegister(KSZ8851_ISR, KSZ8851_ISR_LCIS);
		
		uint16_t status = KSZ8851_ReadRegister(KSZ8851_P1SR);
		
		if ((status & KSZ8851_P1SR_LINK_GOOD) != 0x00)
			ComHdl.SetDebugLinkState(0x01);
		else
			ComHdl.SetDebugLinkState(0x00);
	}
	
	// Packet transmit complete
	if ((isr & KSZ8851_ISR_TXIS) != 0)
	{
		KSZ8851_WriteRegister(KSZ8851_ISR, KSZ8851_ISR_TXIS);
		
		if (ComHdl.DebugLinkState == DEBUG_STATE_TX_PENDING)
			ComHdl.DebugLinkState = DEBUG_STATE_READY;
	}
		
	// Packet received
	if ((isr & KSZ8851_ISR_RXIS) != 0)
	{
		KSZ8851_WriteRegister(KSZ8851_ISR, KSZ8851_ISR_RXIS);
		ComHdl.EthRxPacketCount = (uint8_t)(KSZ8851_ReadRegister(KSZ8851_RXFCTR) >> 8);
	}
	
	// Interrupt Enable Register
	if (!KSZ8851_WriteRegisterAndCheck(
		KSZ8851_IER,
		KSZ8851_IER_LCIE | KSZ8851_IER_TXIE | KSZ8851_IER_RXIE,
		KSZ8851_IER_LCIE | KSZ8851_IER_TXIE | KSZ8851_IER_RXIE))
		return;	
 	return;
}

uint8_t KSZ8851_SendPacketDMAC()
{
	ComHdl.DebugLinkState = DEBUG_STATE_TX_PENDING;
	KSZ8851_SetRegisterBit(KSZ8851_RXQCR, KSZ8851_RXQCR_SDA);
	
	PORT->Group[0].OUTCLR.reg = PORT_PA02;
	
	SERCOM0_TransmitByte(KSZ8851_CMD_WR_FIFO);
		
	EICDisableInterrupt(0);
	SERCOM0->SPI.CTRLB.reg &= ~(SERCOM_SPI_CTRLB_RXEN);
	DMAC_ChannelStartTransfer(DMA_CHID_COM);
	
	return 1;
}

void KSZ8851_SendPacketFinalize()
{
	SERCOM0->SPI.CTRLB.reg |= SERCOM_SPI_CTRLB_RXEN;
	
	PORT->Group[0].OUTSET.reg = PORT_PA02;
		
	KSZ8851_ClearRegisterBit(KSZ8851_RXQCR, KSZ8851_RXQCR_SDA);
	KSZ8851_SetRegisterBit(KSZ8851_TXQCR, KSZ8851_TXQCR_METFE);
	
	EICEnableInterrupt(0);
}

uint16_t KSZ8851_ReceivePacket()
{
	uint16_t byteCount;
	uint16_t packet_status = KSZ8851_ReadRegister(KSZ8851_RXFHSR);
	
	EICDisableInterrupt(0);
		
	if ((packet_status & KSZ8851_RXFHSR_RXFV) != 0x00)
	{
		if((packet_status & (KSZ8851_RXFHSR_RXMR | KSZ8851_RXFHSR_RXFTL | KSZ8851_RXFHSR_RXRF | KSZ8851_RXFHSR_RXCE)) == 0)
		{
			byteCount = KSZ8851_ReadRegister(KSZ8851_RXFHBCR) & 0x0FFF;
			
			KSZ8851_WriteRegister(KSZ8851_RXFDPR, KSZ8851_RXFDPR_RXFPAI);	// Set Pointer to zero
			KSZ8851_SetRegisterBit(KSZ8851_RXQCR, KSZ8851_RXQCR_SDA);		// Start RXQ read
			
			KSZ8851_ReadFifo(RxPacketBuffer, byteCount);
			
			KSZ8851_ClearRegisterBit(KSZ8851_RXQCR, KSZ8851_RXQCR_SDA);	// End RXQ read
		}
	} else
		KSZ8851_SetRegisterBit(KSZ8851_RXQCR, KSZ8851_RXQCR_RRXEF);
	
	EICEnableInterrupt(0);
		
	ComHdl.EthRxPacketCount--;
	
	return byteCount;
}



uint8_t KSZ8851_Init()
{		
	Delay_ms(20);
	PORT->Group[0].OUTSET.reg = PORT_PA17;
	Delay_ms(20);
	
	// Set Host MAC Address
	if (!KSZ8851_WriteRegisterAndCheck(KSZ8851_MARH, KZS8851_MAC_H, KZS8851_MAC_H))
		return 0;
	if (!KSZ8851_WriteRegisterAndCheck(KSZ8851_MARM, KZS8851_MAC_M, KZS8851_MAC_M))
		return 0;
	if (!KSZ8851_WriteRegisterAndCheck(KSZ8851_MARL, KZS8851_MAC_L, KZS8851_MAC_L))
		return 0;
	
	// Transmit Control Register
	if (!KSZ8851_WriteRegisterAndCheck(
		KSZ8851_TXCR,
		KSZ8851_TXCR_TXFCE | KSZ8851_TXCR_TXPE | KSZ8851_TXCR_TXCE,
		KSZ8851_TXCR_TXFCE | KSZ8851_TXCR_TXPE | KSZ8851_TXCR_TXCE))
		return 0;
	
	// TX Frame Data Pointer Register
	if (!KSZ8851_WriteRegisterAndCheck(KSZ8851_TXFDPR, KSZ8851_TXFDPR_TXFPAI, KSZ8851_TXFDPR_TXFPAI))
		return 0;
	
	// Receive Control Register 1
	if (!KSZ8851_WriteRegisterAndCheck(
		KSZ8851_RXCR1,
		KSZ8851_RXCR1_RXPAFMA | KSZ8851_RXCR1_RXMAFMA | KSZ8851_RXCR1_RXME | KSZ8851_RXCR1_RXUE,
		KSZ8851_RXCR1_RXPAFMA | KSZ8851_RXCR1_RXMAFMA | KSZ8851_RXCR1_RXME | KSZ8851_RXCR1_RXUE))
		return 0;
	
	// Receive Control Register 2
	if (!KSZ8851_WriteRegisterAndCheck(
		KSZ8851_RXCR2,
		KSZ8851_RXCR2_SRDBL_SINGLE_FRAME | KSZ8851_RXCR2_IUFFP | KSZ8851_RXCR2_RXIUFCEZ,
		KSZ8851_RXCR2_IUFFP | KSZ8851_RXCR2_RXIUFCEZ))
		return 0;
	
	// RXQ Command Register
	if (!KSZ8851_WriteRegisterAndCheck(
		KSZ8851_RXQCR,
		KSZ8851_RXQCR_RXFCTE | KSZ8851_RXQCR_ADRFE,
		KSZ8851_RXQCR_RXFCTE | KSZ8851_RXQCR_ADRFE))
		return 0;
	
	// RX Frame Data Pointer Register
	if (!KSZ8851_WriteRegisterAndCheck(KSZ8851_RXFDPR, KSZ8851_RXFDPR_RXFPAI, KSZ8851_RXFDPR_RXFPAI))
		return 0;
	
	// RX Frame Count & Threshold Register
	if (!KSZ8851_WriteRegisterAndCheck(KSZ8851_RXFCTR, 0x1, 0x1))
		return 0;
	
	// Flow Control Overrun Watermark Register
	if (!KSZ8851_WriteRegisterAndCheck(KSZ8851_FCOWR, 0x600, 0x600))
		return 0;
	
	// Port 1 Control Register
	KSZ8851_ClearRegisterBit(KSZ8851_P1CR,
		KSZ8851_P1CR_FORCE_DUPLEX				// Force full duplex if (1) AN is disabled or (2) AN is enabled but failed
		);
	// Port 1 Control Register
	KSZ8851_SetRegisterBit(KSZ8851_P1CR,
		KSZ8851_P1CR_RESTART_AN					// Restart auto-negotiation.
		);
	
	// Interrupt Status Register
	KSZ8851_WriteRegister(KSZ8851_ISR, 0xFFFF);
	
	// Interrupt Enable Register
	if (!KSZ8851_WriteRegisterAndCheck(
		KSZ8851_IER,
		KSZ8851_IER_LCIE | KSZ8851_IER_TXIE | KSZ8851_IER_RXIE,
		KSZ8851_IER_LCIE | KSZ8851_IER_TXIE | KSZ8851_IER_RXIE))
		return 0;
	
	// Transmit Control Register
	KSZ8851_SetRegisterBit(KSZ8851_TXCR,
		KSZ8851_TXCR_TXE						// Transmit Enable
		);
	
	// Receive Control Register 1
	KSZ8851_SetRegisterBit(KSZ8851_RXCR1,
		KSZ8851_RXCR1_RXE						// Receive Enable
		);
	
	// Set Packet buffer for DMA transfer
	DMAC_ChannelSetSource((uint32_t*)TxPacketBuffer, sizeof(com_debug_record_t));
	*((uint16_t*)(TxPacketBuffer + 0)) = (KSZ8851_TX_CTRL_TXIC | (0x00 & KSZ8851_TX_CTRL_TXFID));
	*((uint16_t*)(TxPacketBuffer + 2)) = sizeof(com_debug_record_t) - 4;
	
	return 1;
}



void KSZ8851_WriteRegister(uint8_t addr, uint16_t data)
{
	uint8_t cmd[2] = { 0x00, 0x00 };
	
	if((addr & 0x02) != 0)
		cmd[0] = KSZ8851_CMD_WR_REG | KSZ8851_CMD_B3 | KSZ8851_CMD_B2;
	else
		cmd[0] = KSZ8851_CMD_WR_REG | KSZ8851_CMD_B1 | KSZ8851_CMD_B0;
	
	cmd[0] |= (addr >> 6);
	cmd[1] = addr << 2;
	
	PORT->Group[0].OUTCLR.reg = PORT_PA02;
	
	cmd[0] = SERCOM0_TransmitByte(cmd[0]);
	cmd[1] = SERCOM0_TransmitByte(cmd[1]);
	cmd[0] = SERCOM0_TransmitByte((uint8_t)(data & 0xFF));
	cmd[1] = SERCOM0_TransmitByte((uint8_t)((data >> 8) & 0xFF));
	
	PORT->Group[0].OUTSET.reg = PORT_PA02;
}

uint16_t KSZ8851_ReadRegister(uint8_t addr)
{
	uint16_t result = 0x0000;
	uint8_t cmd[2] = { 0x00, 0x00 };
	
	if((addr & 0x02) != 0)
		cmd[0] = KSZ8851_CMD_RD_REG | KSZ8851_CMD_B3 | KSZ8851_CMD_B2;
	else
		cmd[0] = KSZ8851_CMD_RD_REG | KSZ8851_CMD_B1 | KSZ8851_CMD_B0;
	
	cmd[0] |= (addr >> 6);
	cmd[1] = addr << 2;
	
	PORT->Group[0].OUTCLR.reg = PORT_PA02;
	
	cmd[0] = SERCOM0_TransmitByte(cmd[0]);
	cmd[1] = SERCOM0_TransmitByte(cmd[1]);
	result = SERCOM0_TransmitByte(0x00);
	result |= SERCOM0_TransmitByte(0x00) << 8;
	
	PORT->Group[0].OUTSET.reg = PORT_PA02;
	
	return result;
}

void KSZ8851_SetRegisterBit(uint8_t addr, uint16_t mask)
{
	uint16_t value;
	value = KSZ8851_ReadRegister(addr);
	KSZ8851_WriteRegister(addr, value | mask);
}

void KSZ8851_ClearRegisterBit(uint8_t addr, uint16_t mask)
{
	uint16_t value;
	value = KSZ8851_ReadRegister(addr);
	KSZ8851_WriteRegister(addr, value & ~mask);
}

uint8_t KSZ8851_WriteRegisterAndCheck(uint8_t addr, uint16_t data, uint16_t expected)
{
	KSZ8851_WriteRegister(addr, data);
	if (KSZ8851_ReadRegister(addr) == expected)
		return 1;
	else
		return 0;
}

void KSZ8851_ReadFifo(uint8_t *data, uint16_t cnt)
{
	PORT->Group[0].OUTCLR.reg = PORT_PA02;
	
	SERCOM0_TransmitByte(KSZ8851_CMD_RD_FIFO);
	
	// Discard first 8 bytes (4x dummy + 4x RX Header)
	for (uint8_t x = 0; x < 8; x++)
		SERCOM0_TransmitByte(0x00);
	
	for (uint16_t x = 0; x < cnt; x++)
		data[x] = SERCOM0_TransmitByte(0x00);
	
	PORT->Group[0].OUTSET.reg = PORT_PA02;
}