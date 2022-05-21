/*
 * DRV8323Config.cpp
 *
 * Created: 16.05.2022 22:13:54
 *  Author: dominik hellhake
 */ 
#include "DRV8323Config.h"
#include "..\DeviceDriver\SERCOM\SERCOMlib.h"

uint16_t ReadRegister(uint8_t addr)
{
	uint8_t result[2] = {0x80, 0x00};
	result[0] |= (addr & 0b1111) << 3;
	
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA17;

	result[0] = SERCOM0_TransmitByte(result[0]);
	result[1] = SERCOM0_TransmitByte(result[1]);

	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA17;
	
	return ((result[0] << 8) | result[1]);
}

uint16_t WriteRegister(uint8_t addr, uint16_t value)
{
	uint8_t result[2] = {0x00, 0x00};
	result[0] |= (addr & 0b1111) << 3;
	result[0] |= (value >> 8) & 0b111;
	result[1] |= value & 0xFF;
	
	//CS Low
	PORT->Group[0].OUTCLR.reg = PORT_PA17;

	result[0] = SERCOM0_TransmitByte(result[0]);
	result[1] = SERCOM0_TransmitByte(result[1]);

	//CS High
	PORT->Group[0].OUTSET.reg = PORT_PA17;
	
	return ((result[0] << 8) | result[1]);
}

uint8_t VerifyConfiguration()
{
	uint16_t response = 0x0000;
	
	// Configure Driver Control Register
	response = ReadRegister(DRV8323_DCR_Addr);
	if (response != DRV8323_DCR_Val)
		return 0;
	
	// Configure Gate Drive HS Register
	response = ReadRegister(DRV8323_GDHS_Addr);
	if (response != DRV8323_GDHS_Val)
		return 0;
	
	// Configure Gate Drive LS Register
	response = ReadRegister(DRV8323_GDLS_Addr);
	if (response != DRV8323_GDLS_Val)
		return 0;
	
	// Configure OCP Control Register
	response = ReadRegister(DRV8323_OCPCR_Addr);
	if (response != DRV8323_OCPCR_Val)
		return 0;
	
	return 1;
}

uint8_t WriteConfiguration()
{
	// Configure Driver Control Register
	WriteRegister(DRV8323_DCR_Addr, DRV8323_DCR_Val);
	
	// Configure Gate Drive HS Register
	WriteRegister(DRV8323_GDHS_Addr, DRV8323_GDHS_Val);
	
	// Configure Gate Drive LS Register
	WriteRegister(DRV8323_GDLS_Addr, DRV8323_GDLS_Val);
	
	// Configure OCP Control Register
	WriteRegister(DRV8323_OCPCR_Addr, DRV8323_OCPCR_Val);
	
	return 1;
}