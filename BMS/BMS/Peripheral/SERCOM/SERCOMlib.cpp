/* 
* SERCOMlib.cpp
*
* Created: 20.09.2020 13:55:34
* Author: dominik hellhake
*/
#include "SERCOMlib.h"

void InitSERCOM1()
{
	//Enable Clock for SERCOM0
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM1_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM1_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;
	
	
	SERCOM1->SPI.CTRLA.reg =	(0x0 << SERCOM_SPI_CTRLA_DORD_Pos) |
								(0x3 << SERCOM_SPI_CTRLA_MODE_Pos) |
								(0x0 << SERCOM_SPI_CTRLA_DOPO_Pos) |	//MOSI on PAD0; SCK on PAD1
								(0x2 << SERCOM_SPI_CTRLA_DIPO_Pos) |	//MISO on PAD2
								(0x1 << SERCOM_SPI_CTRLA_CPHA_Pos) |
								(0x0 << SERCOM_SPI_CTRLA_CPOL_Pos);
	
	SERCOM1->SPI.CTRLB.reg =	(1 << SERCOM_SPI_CTRLB_RXEN_Pos);
	
	SERCOM1->SPI.BAUD.reg = (48000000 / (2 * 4000000)) - 1;
	
	SERCOM1->SPI.CTRLA.bit.ENABLE = 1;
	while(SERCOM1->SPI.SYNCBUSY.bit.ENABLE);
}
