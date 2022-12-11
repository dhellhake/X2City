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
	/* Init PORT for CAN STDBY */
	PORT->Group[0].DIRSET.reg = PORT_PA27;
	PORT->Group[0].OUTCLR.reg = PORT_PA27;	
	
	PORT->Group[0].DIRSET.reg = PORT_PA25;
	PORT->Group[0].OUTCLR.reg = PORT_PA25;
		
	/* SPI on SERCOM1 */
	SetPinPeripheralFunction(PINMUX_PA16C_SERCOM1_PAD0);	//MOSI
	SetPinPeripheralFunction(PINMUX_PA17C_SERCOM1_PAD1);	//SCK
	SetPinPeripheralFunction(PINMUX_PA18C_SERCOM1_PAD2);	//MISO
	
	/* Init PORT for ADS1118_1->CS */
	PORT->Group[0].DIRSET.reg = PORT_PA19;
	PORT->Group[0].OUTSET.reg = PORT_PA19;
	/* Init PORT for ADS1118_2->CS */
	PORT->Group[0].DIRSET.reg = PORT_PA20;
	PORT->Group[0].OUTSET.reg = PORT_PA20;
	/* Init PORT for ADS1118_3->CS */
	PORT->Group[0].DIRSET.reg = PORT_PA21;
	PORT->Group[0].OUTSET.reg = PORT_PA21;
	
	SetPinPeripheralFunction(PINMUX_PB22G_CAN0_TX);
	SetPinPeripheralFunction(PINMUX_PB23G_CAN0_RX);
}

void SetPinPeripheralFunction(uint32_t pinmux)
{
	uint8_t port = (uint8_t)((pinmux >> 16)/32);
	
	PORT->Group[port].PINCFG[((pinmux >> 16) - (port*32))].bit.PMUXEN = 1;
	
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg &= ~(0xF << (4 * ((pinmux >> 16) & 0x01u)));
	PORT->Group[port].PMUX[((pinmux >> 16) - (port*32))/2].reg |= (uint8_t)((pinmux & 0x0000FFFF) << (4 * ((pinmux >> 16) & 0x01u)));
}