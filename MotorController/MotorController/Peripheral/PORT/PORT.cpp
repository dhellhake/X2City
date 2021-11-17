/*
 * PORT.cpp
 *
 * Created: 15.03.2019 16:38:48
 *  Author: Dominik Hellhake
 */
#include "PORT.h"

void InitPORT()
{
	/* Init PORT for Onboard-LED */
	PORT->Group[0].DIRSET.reg = PORT_PA28;
	PORT->Group[0].OUTCLR.reg = PORT_PA28;
		
	PORT->Group[0].DIRSET.reg = PORT_PA13;
		
	/* Init PORT for USART:*/
	SetPinPeripheralFunction(PINMUX_PB02D_SERCOM5_PAD0);
	SetPinPeripheralFunction(PINMUX_PB03D_SERCOM5_PAD1);
	
	/* Pin-Config: U-Hall on EXTINT13 */
	PORT->Group[0].DIRCLR.reg = PORT_PA25;
	PORT->Group[0].OUTSET.reg = PORT_PA25;
	PORT->Group[0].PINCFG[PIN_PA25].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	SetPinPeripheralFunction(PINMUX_PA25A_EIC_EXTINT13);
	
	/* Pin-Config: V-Hall on EXTINT12 */
	PORT->Group[0].DIRCLR.reg = PORT_PA24;
	PORT->Group[0].OUTSET.reg = PORT_PA24;
	PORT->Group[0].PINCFG[PIN_PA24].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	SetPinPeripheralFunction(PINMUX_PA24A_EIC_EXTINT12);
	
	/* Pin-Config: W-Hall on EXTINT7 */
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