/* 
* SSD1327.cpp
*
* Created: 12.01.2022 19:44:47
* Author: dominik hellhake
*/
#include <math.h>
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
					this->State = OLEDState::InitWireFrame;
				else
					this->State = OLEDState::InitError;
			}
		break;
		case OLEDState::InitWireFrame:
			this->SetWireFrame();			
			this->State = OLEDState::InitMainSection;	
		break;
		case OLEDState::InitMainSection:
			this->InitMainSection();
			this->State = OLEDState::Ready;
		break;
		case OLEDState::Ready:
			if (this->DrawMainSection > 0x00)
				this->UpdateMainSection();
		break;
		case OLEDState::InitError:
		break;
	}
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
uint8_t SSD1327::SetColumnAddress(uint8_t startCol, uint8_t width)
{
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	
	SERCOM1_I2CSendByte(0x00);
	SERCOM1_I2CSendByte(0x15);
	SERCOM1_I2CSendByte(startCol);
	SERCOM1_I2CSendByte(startCol + width);
	
	SERCOM1_I2CStop();
	
	return 1;
}

uint8_t SSD1327::SetRowAddress(uint8_t startRow, uint8_t height)
{	
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	
	SERCOM1_I2CSendByte(0x00);
	SERCOM1_I2CSendByte(0x75);
	SERCOM1_I2CSendByte(startRow);
	SERCOM1_I2CSendByte(startRow + height);
	
	SERCOM1_I2CStop();
	
	return 1;
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
		SERCOM1_I2CStart(OLED_I2C_ADDRESS);
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
	this->SetColumnAddress(0x00, (128 / 2) - 1);
	this->SetRowAddress(0x00, 127);
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 8192; dlen++)
		SERCOM1_I2CSendByte(0x00);
	SERCOM1_I2CStop();
	
	return 1;
}

uint8_t SSD1327::InitMainSection()
{	
	this->SetColumnAddress(24, (16 / 2) - 1);
	this->SetRowAddress(80, 15);	
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 8 * 16; dlen++)
		SERCOM1_I2CSendByte(font16x16[dlen + (20 * (8 * 16))]);
	SERCOM1_I2CStop();
		
	this->SetColumnAddress(24, (16 / 2) - 1);
	this->SetRowAddress(96, 15);
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 8 * 16; dlen++)
		SERCOM1_I2CSendByte(font16x16[dlen + (22 * (8 * 16))]);
	SERCOM1_I2CStop();
	
	this->SetColumnAddress(24, (16 / 2) - 1);
	this->SetRowAddress(112, 15);
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 8 * 16; dlen++)
		SERCOM1_I2CSendByte(font16x16[dlen + (17 * (8 * 16))]);
	SERCOM1_I2CStop();
	
	this->SetColumnAddress(20, (4 / 2) - 1);
	this->SetRowAddress(80, 4);
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 2 * 4; dlen++)
		SERCOM1_I2CSendByte(0xFF);
	SERCOM1_I2CStop();
	
	return 1;
}

uint8_t SSD1327::UpdateMainSection()
{	
	uint8_t fst = (uint8_t)(this->AvlVVeh - fmod(this->AvlVVeh, 10.0f)) / 10.0f;
	uint8_t snd = (uint8_t)fmod(this->AvlVVeh, 10.0f);
	uint8_t thrd = (uint8_t)fmod(this->AvlVVeh*10.0f, 10.0f);
	
	this->SetColumnAddress(12, (40 / 2) - 1);
	this->SetRowAddress(0, 40);		
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 20*40; dlen++)
		SERCOM1_I2CSendByte(font40x40[dlen + (fst * (20 * 40))]);
	SERCOM1_I2CStop();
	
	this->SetColumnAddress(12, (40 / 2) - 1);
	this->SetRowAddress(40, 40);
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 20*40; dlen++)
		SERCOM1_I2CSendByte(font40x40[dlen + (snd * (20 * 40))]);
	SERCOM1_I2CStop();
		
	this->SetColumnAddress(15, (16 / 2) - 1);
	this->SetRowAddress(84, 16);
	SERCOM1_I2CStart(OLED_I2C_ADDRESS);
	SERCOM1_I2CSendByte(0x40);
	for (uint16_t dlen = 0; dlen < 8*16; dlen++)
		SERCOM1_I2CSendByte(font16x16[dlen + (thrd * (8 * 16))]);
	SERCOM1_I2CStop();
	
	this->DrawMainSection = 0;
	return 1;
}


uint8_t SSD1327::SetWireFrame()
{
	this->SetRowAddress(OLED_SEPARATOR_START_ROW, OLED_SEPARATOR_HEIGHT);	
	
	uint8_t separatorPos[3] = { 
		OLED_SEPARATOR_1_START_COL, 
		OLED_SEPARATOR_2_START_COL, 
		OLED_SEPARATOR_3_START_COL 
	};
	
	for (uint8_t x = 0; x < 3; x++)
	{			
		this->SetColumnAddress(separatorPos[x], 0);
				
		SERCOM1_I2CStart(OLED_I2C_ADDRESS);
		SERCOM1_I2CSendByte(0x40);
		for (uint16_t dlen = 0; dlen < 128; dlen++)
			SERCOM1_I2CSendByte(0x40);
		SERCOM1_I2CStop();
	}
	
	return 1;
}