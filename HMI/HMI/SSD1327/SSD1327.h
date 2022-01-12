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

typedef enum OLEDState
{
	PowerUp =			0x00,
	Initialized =		0x01,
	Ready =				0x02,
	
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
	
	
	private:
		OLEDState State = OLEDState::PowerUp;
		
		uint8_t Initialize();
		uint8_t ClearFrame();
		uint8_t UpdateMainSection();
		uint8_t SetWireFrame();
	
}; //SSD1327

extern SSD1327 OLED;

#endif //__SSD1327_H__
