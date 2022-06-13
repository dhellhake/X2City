/*
* DMAClib.cpp
*
* Created: 04.09.2021 17:43:00
* Author: dominik hellhake
*/
#include "DMAClib.h"
#include "..\..\BLDC\BLDC.h"

#define COMPILER_ALIGNED(a)        __attribute__((__aligned__(a)))

COMPILER_ALIGNED(16)
DmacDescriptor descriptor_section[3];
COMPILER_ALIGNED(16)
static DmacDescriptor write_back_section[3];

void InitDMACChannel0();
void InitDMACChannel1();
void InitDMACChannel2();

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
	
	InitDMACChannel0();		// BLDC->U for Sinusoidal PWM
	InitDMACChannel1();		// BLDC->V for Sinusoidal PWM
	InitDMACChannel2();		// BLDC->W for Sinusoidal PWM
}

void InitDMACChannel0()
{
	DMAC->CHID.reg =					DMAC_CHID_ID(0);										// Select Channel 0 for configuration
	DMAC->CHCTRLA.reg &=				~DMAC_CHCTRLA_ENABLE;									// Disable Channel
	DMAC->CHCTRLA.reg =					DMAC_CHCTRLA_SWRST;										// Software Reset
	
	DMAC->CHCTRLB.reg =					DMAC_CHCTRLB_LVL(0) |									// Arbitration Level 0
										DMAC_CHCTRLB_TRIGSRC(TC3_DMAC_ID_OVF) | 				// TC0 OVF trigger
										DMAC_CHCTRLB_TRIGACT(DMAC_CHCTRLB_TRIGACT_BEAT_Val);	// Trigger Block Transfer
	
	descriptor_section[0].BTCTRL.reg =	(0x2 << DMAC_BTCTRL_STEPSIZE_Pos) |						// Set StepSitze to 1x => ADDR = ADDR + (Beat size in byte) * 1
										(0x0 << DMAC_BTCTRL_STEPSEL_Pos) |						// Only Source-Address is incremented by StepSize
										(0x0 << DMAC_BTCTRL_DSTINC_Pos) |						// Destination Address Increment disabled
										(0x1 << DMAC_BTCTRL_SRCINC_Pos) |						// Source Address Increment enabled
										(0x2 << DMAC_BTCTRL_BEATSIZE_Pos) |						// Beat Size is 4 Byte
										(0x0 << DMAC_BTCTRL_BLOCKACT_Pos) |						// NoAction after Block Transfer
										(0x0 << DMAC_BTCTRL_EVOSEL_Pos) |						// Event Output Disabled
										(0x1 << DMAC_BTCTRL_VALID_Pos);							// Descriptor is Valid
	
	descriptor_section[0].BTCNT.reg =	sizeof(DRV.TarSineDuty) / 4;							// Set Bytes to transfer
	descriptor_section[0].SRCADDR.reg = (uint32_t)DRV.TarSineDuty + sizeof(DRV.TarSineDuty);	// Set Source Address
	descriptor_section[0].DSTADDR.reg = (uint32_t)(&TCC0->CCBUF[3].reg);						// Set Destination Address
	
	descriptor_section[0].DESCADDR.reg = 0;
}

void InitDMACChannel1()
{
	DMAC->CHID.reg =					DMAC_CHID_ID(1);										// Select Channel 1 for configuration
	DMAC->CHCTRLA.reg &=				~DMAC_CHCTRLA_ENABLE;									// Disable Channel
	DMAC->CHCTRLA.reg =					DMAC_CHCTRLA_SWRST;										// Software Reset
	
	DMAC->CHCTRLB.reg =					DMAC_CHCTRLB_LVL(0) |									// Arbitration Level 0
										DMAC_CHCTRLB_TRIGSRC(TC3_DMAC_ID_OVF) | 				// TC0 OVF trigger
										DMAC_CHCTRLB_TRIGACT(DMAC_CHCTRLB_TRIGACT_BEAT_Val);	// Trigger Block Transfer
	
	descriptor_section[1].BTCTRL.reg =	(0x2 << DMAC_BTCTRL_STEPSIZE_Pos) |						// Set StepSitze to 1x => ADDR = ADDR + (Beat size in byte) * 1
										(0x0 << DMAC_BTCTRL_STEPSEL_Pos) |						// Only Source-Address is incremented by StepSize
										(0x0 << DMAC_BTCTRL_DSTINC_Pos) |						// Destination Address Increment disabled
										(0x1 << DMAC_BTCTRL_SRCINC_Pos) |						// Source Address Increment enabled
										(0x2 << DMAC_BTCTRL_BEATSIZE_Pos) |						// Beat Size is 4 Byte
										(0x0 << DMAC_BTCTRL_BLOCKACT_Pos) |						// NoAction after Block Transfer
										(0x0 << DMAC_BTCTRL_EVOSEL_Pos) |						// Event Output Disabled
										(0x1 << DMAC_BTCTRL_VALID_Pos);							// Descriptor is Valid
	
	descriptor_section[1].BTCNT.reg =	sizeof(DRV.TarSineDuty) / 4;							// Set Bytes to transfer
	descriptor_section[1].SRCADDR.reg = (uint32_t)DRV.TarSineDuty + sizeof(DRV.TarSineDuty);	// Set Source Address
	descriptor_section[1].DSTADDR.reg = (uint32_t)(&TCC0->CCBUF[2].reg);						// Set Destination Address
	
	descriptor_section[1].DESCADDR.reg = 0;
}

void InitDMACChannel2()
{
	DMAC->CHID.reg =					DMAC_CHID_ID(2);										// Select Channel 1 for configuration
	DMAC->CHCTRLA.reg &=				~DMAC_CHCTRLA_ENABLE;									// Disable Channel
	DMAC->CHCTRLA.reg =					DMAC_CHCTRLA_SWRST;										// Software Reset
	
	DMAC->CHCTRLB.reg =					DMAC_CHCTRLB_LVL(0) |									// Arbitration Level 0
										DMAC_CHCTRLB_TRIGSRC(TC3_DMAC_ID_OVF) | 				// TC0 OVF trigger
										DMAC_CHCTRLB_TRIGACT(DMAC_CHCTRLB_TRIGACT_BEAT_Val);	// Trigger Block Transfer
	
	descriptor_section[2].BTCTRL.reg =	(0x2 << DMAC_BTCTRL_STEPSIZE_Pos) |						// Set StepSitze to 1x => ADDR = ADDR + (Beat size in byte) * 1
										(0x0 << DMAC_BTCTRL_STEPSEL_Pos) |						// Only Source-Address is incremented by StepSize
										(0x0 << DMAC_BTCTRL_DSTINC_Pos) |						// Destination Address Increment disabled
										(0x1 << DMAC_BTCTRL_SRCINC_Pos) |						// Source Address Increment enabled
										(0x2 << DMAC_BTCTRL_BEATSIZE_Pos) |						// Beat Size is 4 Byte
										(0x0 << DMAC_BTCTRL_BLOCKACT_Pos) |						// NoAction after Block Transfer
										(0x0 << DMAC_BTCTRL_EVOSEL_Pos) |						// Event Output Disabled
										(0x1 << DMAC_BTCTRL_VALID_Pos);							// Descriptor is Valid
	
	descriptor_section[2].BTCNT.reg =	sizeof(DRV.TarSineDuty) / 4;							// Set Bytes to transfer
	descriptor_section[2].SRCADDR.reg = (uint32_t)DRV.TarSineDuty + sizeof(DRV.TarSineDuty);	// Set Source Address
	descriptor_section[2].DSTADDR.reg = (uint32_t)(&TCC0->CCBUF[0].reg);						// Set Destination Address
	
	descriptor_section[2].DESCADDR.reg = 0;
}