/*
 * PORT.cpp
 *
 * Created: 15.03.2019 16:38:48
 *  Author: Dominik Hellhake
 */
#include "PORT.h"

void InitPORT()
{
	PORT->Group[0].DIRSET.reg = PORT_PA11;
	PORT->Group[0].OUTSET.reg = PORT_PA11;
	PORT->Group[0].DIRSET.reg = PORT_PA13;
	PORT->Group[0].OUTCLR.reg = PORT_PA13;
	
	// Pin-Config: Onboard-LED
	PORT->Group[0].DIRSET.reg = PORT_PA28;
	PORT->Group[0].OUTCLR.reg = PORT_PA28;
	
	/* SPI on SERCOM0 */
	SetPinPeripheralFunction(PINMUX_PA08C_SERCOM0_PAD0);	//MISO
	SetPinPeripheralFunction(PINMUX_PA09C_SERCOM0_PAD1);	//MOSI
	SetPinPeripheralFunction(PINMUX_PA10C_SERCOM0_PAD2);	//SCK
	
	
	/* KZS8851 */	
	// Pin-Config: KSZ8851->nRST on PA03
	PORT->Group[0].DIRSET.reg = PORT_PA03;
	PORT->Group[0].OUTCLR.reg = PORT_PA03;
	
	// Pin-Config: KSZ8851->CS on PA02
	PORT->Group[0].DIRSET.reg = PORT_PA02;
	PORT->Group[0].OUTSET.reg = PORT_PA02;
	
	// Pin-Config: KSZ8851 on PB08
	PORT->Group[1].DIRCLR.reg = PORT_PB08;
	PORT->Group[1].OUTSET.reg = PORT_PB08;
	SetPinPeripheralFunction(PINMUX_PB08A_EIC_EXTINT8);
	
	
	/* DRV8323 */
	// Pin-Config: DRV8323->CS on PA17
	PORT->Group[0].DIRSET.reg = PORT_PA17;
	PORT->Group[0].OUTSET.reg = PORT_PA17;
	
	// Pin-Config: DRV8323->nFAULT on PA16
	PORT->Group[1].DIRCLR.reg = PORT_PA16;
	PORT->Group[1].OUTSET.reg = PORT_PA16;
	//PORT->Group[0].PINCFG[PORT_PA17].reg = PORT_PINCFG_INEN | PORT_PINCFG_PULLEN;
	SetPinPeripheralFunction(PINMUX_PA16A_EIC_EXTINT0);
	
	
	/* Hall Sensor */	
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
	
		
	/* BLDC Out */
	//Configure PWM Output for U
	PORT->Group[0].DIRSET.reg = PORT_PA19;
	PORT->Group[0].OUTCLR.reg = PORT_PA19;
	SetPinPeripheralFunction(PINMUX_PA19F_TCC0_WO3);	// High
	
	PORT->Group[0].DIRSET.reg = PORT_PA21;
	PORT->Group[0].OUTCLR.reg = PORT_PA21;
	SetPinPeripheralFunction(PINMUX_PA21F_TCC0_WO7);	// Low
	
	
	//Configure PWM Output for V
	PORT->Group[0].DIRSET.reg = PORT_PA18;
	PORT->Group[0].OUTCLR.reg = PORT_PA18;
	SetPinPeripheralFunction(PINMUX_PA18F_TCC0_WO2);	// High
	
	PORT->Group[0].DIRSET.reg = PORT_PA20;
	PORT->Group[0].OUTCLR.reg = PORT_PA20;
	SetPinPeripheralFunction(PINMUX_PA20F_TCC0_WO6);	// Low
	
	
	//Configure PWM Output for W
	PORT->Group[0].DIRSET.reg = PORT_PA04;
	PORT->Group[0].OUTCLR.reg = PORT_PA04;
	SetPinPeripheralFunction(PINMUX_PA04E_TCC0_WO0);	// High
	
	PORT->Group[0].DIRSET.reg = PORT_PA22;
	PORT->Group[0].OUTCLR.reg = PORT_PA22;
	SetPinPeripheralFunction(PINMUX_PA22F_TCC0_WO4);	// Low
}

void SetPinPeripheralFunction(uint32_t pinmux)
{
	uint8_t port = (uint8_t)((pinmux >> 16)/32);
	
	PORT->Group[port].PINCFG[((pinmux >> 16) - (port*32))].bit.PMUXEN = 1;
	
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg &= ~(0xF << (4 * ((pinmux >> 16) & 0x01u)));
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg |= (uint8_t)((pinmux & 0x0000FFFF) << (4 * ((pinmux >> 16) & 0x01u)));
}