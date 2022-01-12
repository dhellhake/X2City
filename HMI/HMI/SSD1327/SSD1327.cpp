/* 
* SSD1327.cpp
*
* Created: 12.01.2022 19:44:47
* Author: dominik hellhake
*/
#include "SSD1327.h"
#include "font.h"
#include "..\Peripheral\SERCOM\SERCOMlib.h"

SSD1327 OLED;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT SSD1327::Run(uint32_t timeStamp)
{
	
	switch(this->State)
	{
		case OLEDState::PowerUp:
			if (this->Initialize() > 0x00)
			{
				if (this->ClearFrame() > 0x00)
					this->State = OLEDState::Initialized;
				else
					this->State = OLEDState::InitError;
			}
		break;
		case OLEDState::Initialized:
			this->SetWireFrame();			
			this->State = OLEDState::Ready;	
		break;
		case OLEDState::Ready:
			this->UpdateMainSection();
			this->State = OLEDState::InitError;
		break;
		case OLEDState::InitError:
		break;
	}
	
	return RUN_RESULT::SUCCESS;
}

uint8_t SSD1327::Initialize()
{
	uint8_t init_seq[39] = {
		0x2, 0xfd, 0x12,		// unlock the controller
		0x2, 0xb3, 0x91,		// set clock divider
		0x2, 0xca, 0x3f,		// set COMS multiplex ratio 1/64
		0x2, 0xa2, 0x00,		// set display offset
		0x2, 0xa1, 0x00,		// set display start line
		0x2, 0xa0, 0x50,		// set display remap
		0x2, 0xb5, 0x00,		// disable GPIO
		0x2, 0xab, 0x01,		// select external VDD regulator (none)
		0x3, 0xb4, 0xa0, 0xfd,	// external VSL display enhancement
		0x2, 0xb1, 0xe2,		// set phase length
		0x3, 0xd1, 0x82, 0x20,	// display enahancement B
		0x1, 0xa4,				// set normal display mode
		0x1, 0xaf 				// Display ON in normal mode
	};
	
	for (uint8_t dataIdx = 0; dataIdx < 39;)
	{
		uint8_t dataLen = init_seq[dataIdx++];
		SERCOM1_I2CStart(0x78);
		SERCOM1_I2CSendByte(0x00);
		for (uint16_t dlen = 0; dlen < dataLen; dlen++)
			SERCOM1_I2CSendByte(init_seq[dataIdx + dlen]);
		SERCOM1_I2CStop();
		dataIdx += dataLen;
	}
	
	return 1;
}

uint8_t SSD1327::ClearFrame()
{
	SERCOM1_I2CStart(0x78);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 8192; dlen++)
		SERCOM1_I2CSendByte(0x00);
	SERCOM1_I2CStop();
	
	return 1;
}

uint8_t SSD1327::UpdateMainSection()
{
	uint8_t setColAddr[4] = { 0x00, 0x15, 12,  12+19};
	SERCOM1_I2CStart(0x78);
	for (uint16_t dlen = 0; dlen < 4; dlen++)
		SERCOM1_I2CSendByte(setColAddr[dlen]);
	SERCOM1_I2CStop();
	
	uint8_t setRowAddr[4] = { 0x00, 0x75, 0x00,  40};
	SERCOM1_I2CStart(0x78);
	for (uint16_t dlen = 0; dlen < 4; dlen++)
		SERCOM1_I2CSendByte(setRowAddr[dlen]);
	SERCOM1_I2CStop();
	
	SERCOM1_I2CStart(0x78);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 20*40; dlen++)
		SERCOM1_I2CSendByte(example[dlen + (4 * (20 * 40))]);
	SERCOM1_I2CStop();
	
	return 1;
}

uint8_t SSD1327::SetWireFrame()
{	
	uint8_t setRowAddr[4] = { 0x00, 0x75, 0x00,  127};
	SERCOM1_I2CStart(0x78);
	for (uint16_t dlen = 0; dlen < 4; dlen++)
		SERCOM1_I2CSendByte(setRowAddr[dlen]);
	SERCOM1_I2CStop();
	
	uint8_t separatorPos[3] = { 9, 33, 49 };
	for (uint8_t x = 0; x < 3; x++)
	{		
		uint8_t setColAddr[4] = { 0x00, 0x15, separatorPos[x],  separatorPos[x]};
		SERCOM1_I2CStart(0x78);
		for (uint16_t dlen = 0; dlen < 4; dlen++)
			SERCOM1_I2CSendByte(setColAddr[dlen]);
		SERCOM1_I2CStop();
				
		SERCOM1_I2CStart(0x78);
		SERCOM1_I2CSendByte(0x40);
		for (uint16_t dlen = 0; dlen < 128; dlen++)
			SERCOM1_I2CSendByte(0x40);
		SERCOM1_I2CStop();
	}
	
	return 1;
}