/*
 * PORT.cpp
 *
 * Created: 15.03.2019 16:38:48
 *  Author: Dominik Hellhake
 */
#include "PORT.h"

void InitPORT()
{
	
	/* I/O */
	// Pin-Config: Onboard-LED
	PORT->Group[0].DIRSET.reg = PORT_PA28;
	PORT->Group[0].OUTCLR.reg = PORT_PA28;
	
	// Pin-Config: KSZ8851->nRST on PA17
	PORT->Group[0].DIRSET.reg = PORT_PA17;
	PORT->Group[0].OUTCLR.reg = PORT_PA17;
	
	// Pin-Config: KSZ8851->CS on PA02
	PORT->Group[0].DIRSET.reg = PORT_PA02;
	PORT->Group[0].OUTSET.reg = PORT_PA02;
	
	// Pin-Config: DRV8323->CS on PA03
	PORT->Group[0].DIRSET.reg = PORT_PA03;
	PORT->Group[0].OUTSET.reg = PORT_PA03;
	
	/* SPI on SERCOM0 */	
	SetPinPeripheralFunction(PINMUX_PA08C_SERCOM0_PAD0);	//MISO
	SetPinPeripheralFunction(PINMUX_PA09C_SERCOM0_PAD1);	//MOSI
	SetPinPeripheralFunction(PINMUX_PA10C_SERCOM0_PAD2);	//SCK
	
	
	/* External Interrupt */
	// Pin-Config: KSZ8851 on EXTINT0
	PORT->Group[0].DIRCLR.reg = PORT_PA16;
	PORT->Group[0].OUTSET.reg = PORT_PA16;
	SetPinPeripheralFunction(PINMUX_PA16A_EIC_EXTINT0);
	
	// Pin-Config: U-Hall on EXTINT13
	PORT->Group[0].DIRCLR.reg = PORT_PA25;
	PORT->Group[0].OUTSET.reg = PORT_PA25;
	PORT->Group[0].PINCFG[PIN_PA25].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	SetPinPeripheralFunction(PINMUX_PA25A_EIC_EXTINT13);
	
	// Pin-Config: V-Hall on EXTINT12
	PORT->Group[0].DIRCLR.reg = PORT_PA24;
	PORT->Group[0].OUTSET.reg = PORT_PA24;
	PORT->Group[0].PINCFG[PIN_PA24].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	SetPinPeripheralFunction(PINMUX_PA24A_EIC_EXTINT12);
	
	// Pin-Config: W-Hall on EXTINT7
	PORT->Group[0].DIRCLR.reg = PORT_PA23;
	PORT->Group[0].OUTSET.reg = PORT_PA23;
	PORT->Group[0].PINCFG[PIN_PA23].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	SetPinPeripheralFunction(PINMUX_PA23A_EIC_EXTINT7);
}

void SetPinPeripheralFunction(uint32_t pinmux)
{
	uint8_t port = (uint8_t)((pinmux >> 16)/32);
	
	PORT->Group[port].PINCFG[((pinmux >> 16) - (port*32))].bit.PMUXEN = 1;
	
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg &= ~(0xF << (4 * ((pinmux >> 16) & 0x01u)));
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg |= (uint8_t)((pinmux & 0x0000FFFF) << (4 * ((pinmux >> 16) & 0x01u)));
}