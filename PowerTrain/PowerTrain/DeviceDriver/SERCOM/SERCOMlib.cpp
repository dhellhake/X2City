/* 
* SERCOMlib.cpp
*
* Created: 20.09.2020 13:55:34
* Author: dominik hellhake
*/
#include "SERCOMlib.h"

void InitSERCOM0()
{
//Enable Clock for SERCOM0
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM0_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM0_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;
	
	
	SERCOM0->SPI.CTRLA.reg =	(0x3 << SERCOM_SPI_CTRLA_MODE_Pos) |
								(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |	//MOSI on PAD0; SCK on PAD1
								(0x2 << SERCOM_SPI_CTRLA_DIPO_Pos) |	//MISO on PAD2
								(0x1 << SERCOM_SPI_CTRLA_CPHA_Pos) |
								(0x0 << SERCOM_SPI_CTRLA_CPOL_Pos);
	
	SERCOM0->SPI.CTRLB.reg =	(1 << SERCOM_SPI_CTRLB_RXEN_Pos);
	
	SERCOM0->SPI.BAUD.reg = (48000000 / (2 * 500000)) - 1;
	
	SERCOM0->SPI.CTRLA.bit.ENABLE = 1;
	while(SERCOM0->SPI.SYNCBUSY.bit.ENABLE);
}

void SERCOM0_SetBAUD(uint32_t fbaud)
{
	SERCOM0->SPI.BAUD.reg = (48000000 / (2 * fbaud)) - 1;
}