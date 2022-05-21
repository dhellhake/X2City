/*
* DMAClib.cpp
*
* Created: 04.09.2021 17:43:00
* Author: dominik hellhake
*/
#include "DMAClib.h"


#define COMPILER_ALIGNED(a)        __attribute__((__aligned__(a)))

COMPILER_ALIGNED(16)
DmacDescriptor descriptor_section[4];
COMPILER_ALIGNED(16)
static DmacDescriptor write_back_section[4];

void InitDMACChannel0();

void InitDMAC()
{
	MCLK->AHBMASK.reg |= MCLK_AHBMASK_DMAC;
	
	
	/* Configuration of DMAC */
	DMAC->CTRL.reg &=		~DMAC_CTRL_DMAENABLE;							// Disable DMAC
	DMAC->CTRL.reg =		DMAC_CTRL_SWRST;								// Software Reset
	
	DMAC->BASEADDR.reg = (uint32_t)descriptor_section;						// Set Descriptor address
	DMAC->WRBADDR.reg = (uint32_t)write_back_section;						// Set Write-Back address
	
	DMAC->CTRL.reg =		DMAC_CTRL_DMAENABLE |							// Enable all priority level
							DMAC_CTRL_LVLEN(0xf);							// Enable DMAC
	
	NVIC_SetPriority(DMAC_IRQn, 1);
	NVIC_EnableIRQ(DMAC_IRQn);		// Enable SysTick Interrupt
	
	InitDMACChannel0();
}

void InitDMACChannel0()
{
	DMAC->CHID.reg =					DMAC_CHID_ID(0);										// Select Channel 0 for configuration
	DMAC->CHCTRLA.reg &=				~DMAC_CHCTRLA_ENABLE;									// Disable Channel
	DMAC->CHCTRLA.reg =					DMAC_CHCTRLA_SWRST;										// Software Reset
	
	DMAC->CHINTENSET.reg =				DMAC_CHINTENSET_TERR |									// Channel Transfer Error Interrupt Enable
										DMAC_CHINTENSET_TCMPL |									// Channel Transfer Complete Interrupt Enable
										DMAC_CHINTENSET_SUSP;									// Channel Suspend Interrupt Enable
	
	DMAC->CHCTRLB.reg =					DMAC_CHCTRLB_LVL(0) |									// Arbitration Level 0
										DMAC_CHCTRLB_TRIGSRC(SERCOM0_DMAC_ID_TX) | 				// Sercom5 TX trigger
										DMAC_CHCTRLB_TRIGACT(DMAC_CHCTRLB_TRIGACT_BEAT_Val);	// Trigger Block Transfer
	
	descriptor_section[0].BTCTRL.reg =	(0x0 << DMAC_BTCTRL_STEPSIZE_Pos) |						// Set StepSitze to 1x => ADDR = ADDR + (Beat size in byte) * 1
										(0x0 << DMAC_BTCTRL_STEPSEL_Pos) |						// Only Source-Address is incremented by StepSize
										(0x0 << DMAC_BTCTRL_DSTINC_Pos) |						// Destination Address Increment disabled
										(0x1 << DMAC_BTCTRL_SRCINC_Pos) |						// Source Address Increment enabled
										(0x0 << DMAC_BTCTRL_BEATSIZE_Pos) |						// Beat Size is 1 Byte
										(0x1 << DMAC_BTCTRL_BLOCKACT_Pos) |						// NoAction after Block Transfer
										(0x0 << DMAC_BTCTRL_EVOSEL_Pos) |						// Event Output Disabled
										(0x1 << DMAC_BTCTRL_VALID_Pos);							// Descriptor is Valid
	
	
	descriptor_section[0].DSTADDR.reg = (uint32_t)(&SERCOM0->SPI.DATA.reg);						// Set Destination Address	
	descriptor_section[0].DESCADDR.reg = 0;														// No successor descriptor
}

void DMAC_ChannelSetSource(uint32_t* data, uint16_t len)
{
	descriptor_section[0].BTCNT.reg =	len;													// Set Bytes to transfer
	descriptor_section[0].SRCADDR.reg = (uint32_t)data + len;									// Set Source Address
}

void DMAC_Handler()
{
	if ((DMAC->CHINTFLAG.reg	& DMAC_CHINTFLAG_TCMPL) != 0x00)
	{
		DMAC->CHINTFLAG.reg = DMAC_CHINTFLAG_TCMPL;
	}
}