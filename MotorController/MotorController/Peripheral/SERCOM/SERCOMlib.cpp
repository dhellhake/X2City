/* 
* SERCOMlib.cpp
*
* Created: 20.09.2020 13:55:34
* Author: dominik hellhake
*/
#include "SERCOMlib.h"

void InitSERCOM5()
{
	uint64_t br = (uint64_t)65536 * (48000000 - 16 * 921600) / 48000000;	// Variable for baud rate
	
	//Enable Clock for SERCOM5
	//Set bits in the clock mask for an APBx bus.
	MCLK->APBCMASK.bit.SERCOM5_ = 1;
	
	/* Disable the peripheral channel */
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg &= ~GCLK_PCHCTRL_CHEN;
	while (GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg & GCLK_PCHCTRL_CHEN);

	/* Configure the peripheral channel */
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg = GCLK_PCHCTRL_GEN(0);

	// Enable GCLK for peripheral
	GCLK->PCHCTRL[SERCOM5_GCLK_ID_CORE].reg |= GCLK_PCHCTRL_CHEN;
		
	
	SERCOM5->USART.CTRLA.reg =	(1 << SERCOM_USART_CTRLA_DORD_Pos) |
								(0 << SERCOM_USART_CTRLA_CPOL_Pos) |
								(0 << SERCOM_USART_CTRLA_CMODE_Pos) |
								(1 << SERCOM_USART_CTRLA_FORM_Pos) |
								(0 << SERCOM_USART_CTRLA_SAMPA_Pos) |
								(1 << SERCOM_USART_CTRLA_RXPO_Pos) |	//Tx => PAD0; Rx => PAD1
								(0 << SERCOM_USART_CTRLA_TXPO_Pos) |	//Tx => PAD0; Rx => PAD1
								(0 << SERCOM_USART_CTRLA_SAMPR_Pos) |
								(0 << SERCOM_USART_CTRLA_IBON_Pos) |
								(0 << SERCOM_USART_CTRLA_RUNSTDBY_Pos) |
								(1 << SERCOM_USART_CTRLA_MODE_Pos);
	
	SERCOM5->USART.CTRLB.reg =	(1 << SERCOM_USART_CTRLB_RXEN_Pos) |
								(1 << SERCOM_USART_CTRLB_TXEN_Pos) |
								(0 << SERCOM_USART_CTRLB_PMODE_Pos) |
								(0 << SERCOM_USART_CTRLB_ENC_Pos) |
								(0 << SERCOM_USART_CTRLB_SFDE_Pos) |
								(0 << SERCOM_USART_CTRLB_COLDEN_Pos) |
								(0 << SERCOM_USART_CTRLB_SBMODE_Pos) |
								(0 << SERCOM_USART_CTRLB_CHSIZE_Pos);
	
	SERCOM5->USART.BAUD.reg = (uint16_t)br;
	
	SERCOM5->USART.CTRLA.reg |= (1 << SERCOM_USART_CTRLA_ENABLE_Pos);
}

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