/* 
* SSD1327.h
*
* Created: 12.01.2022 19:44:47
* Author: dominik hellhake
*/
#ifndef __SSD1327_H__
#define __SSD1327_H__

#include "sam.h"
#include "..\Task.h"

#define OLED_I2C_ADDRESS				0x78

/* Main Section */
#define OLED_MAINSECTION_START_ROW		0
#define OLED_MAINSECTION_START_COL		12
#define OLED_MAINSECTION_HEIGHT			40

/* Separator */
#define OLED_SEPARATOR_START_ROW		0
#define OLED_SEPARATOR_HEIGHT			127
#define OLED_SEPARATOR_1_START_COL		9
#define OLED_SEPARATOR_2_START_COL		33
#define OLED_SEPARATOR_3_START_COL		49


typedef enum OLEDState
{
	PowerUp =			0x00,
	InitWireFrame =		0x01,
	InitMainSection =	0x02,
	
	Ready =				0x20,
	
	InitError =			0xFF
} OLEDState;


class SSD1327 : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	public:
		inline void SetVVeh(float vVeh)
		{
			if (this->AvlVVeh != vVeh)			
				this->DrawMainSection = 0x1;
			
			if (vVeh >= 100)
				this->AvlVVeh = 99.9f;
			else
				this->AvlVVeh = vVeh;
		}
	private:
		float AvlVVeh	= 34.56f;
		uint8_t DrawMainSection = 0x1;
	
		OLEDState State = OLEDState::PowerUp;
		
		uint8_t Initialize();
		uint8_t ClearFrame();
		uint8_t SetWireFrame();
		
		uint8_t InitMainSection();
		uint8_t UpdateMainSection();
		
		uint8_t SetColumnAddress(uint8_t startCol, uint8_t width);
		uint8_t SetRowAddress(uint8_t startRow, uint8_t height);
	
}; //SSD1327

extern SSD1327 OLED;

#endif //__SSD1327_H__
