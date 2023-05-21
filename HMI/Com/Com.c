/*
 * Com.c
 *
 * Created: 21.05.2023 18:03:10
 *  Author: dominik hellhake
 */ 
#include "Com.h"
#include "../Peripheral/CAN/CANlib.h"

uint32_t Com_Init()
{
	CAN0_Start();
	return 1;	
}

void Com_Rx_Cyclic()
{
	uint16_t identifier;
	uint8_t message[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	uint8_t messageCnt = can0_fifo0_data_available();
	
	for (;messageCnt > 0; messageCnt--)
	{
			identifier = can0_fifo0_read_message(message);
			identifier++;
	}	
}