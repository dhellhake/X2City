/* 
* CANlib.h
*
* Created: 16.01.2022 10:02:22
* Author: dominik hellhake
*/
#ifndef __CANLIB_H__
#define __CANLIB_H__

#include "samc21.h"

#define CAN_FILTER_ID_MOTORCONTROL			0x45a
#define CAN_FILTER_ID_BATTERYCONTROL        0x25a


void InitCAN0();

void CAN0_Start();

void can0_transmit(uint8_t size, uint8_t* data);

uint8_t can0_fifo0_data_available();
uint16_t can0_fifo0_read_message(uint8_t* buffer);

#endif //__CANLIB_H__
