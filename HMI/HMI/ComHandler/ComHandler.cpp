/* 
* ComHandler.cpp
*
* Created: 16.01.2022 13:04:30
* Author: dominik hellhake
*/
#include "ComHandler.h"
#include "..\SSD1327\SSD1327.h"

ComHandler ComHdl;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ComHandler::Run(uint32_t timeStamp)
{
	float v_veh = 0.0f;
	for (uint8_t x = 0; x < 4; x++)
		((uint8_t*)&v_veh)[x] = this->can_motorcontrol_message_buffer[x];
	
	OLED.SetVVeh(v_veh);
	
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
