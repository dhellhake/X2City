/* 
* ComHandler.cpp
*
* Created: 15.03.2022 20:07:15
* Author: dominik hellhake
*/
#include "ComHandler.h"
#include "..\RuntimeEnvironment\RuntimeEnvironment.h"
#include "..\DeviceDriver\CAN\CANlib.h"

#define CAN_FILTER_ID_MOTORCONTROL			0x45a
#define CAN_FILTER_ID_BATTERYCONTROL        0x25a

ComHandler ComHdl;

uint32_t tme[100];
uint8_t tmeIdx = 0x00;

/************************************************************************/
/* Executable Interface implementation                                  */
/************************************************************************/
RUN_RESULT ComHandler::Run(uint32_t timeStamp)
{		
	can0_com();
	
	//can0_transmit(CAN_FILTER_ID_MOTORCONTROL, sizeof(rte_image_record_t), (uint8_t*)&Rte.Record);
	
	return RUN_RESULT::SUCCESS;
}

/************************************************************************/
/* Class implementation                                                 */
/************************************************************************/
