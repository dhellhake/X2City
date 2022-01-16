/* 
* CANlib.h
*
* Created: 16.01.2022 10:02:22
* Author: dominik hellhake
*/
#ifndef __CANLIB_H__
#define __CANLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include <stdalign.h>
	#include "samc21.h"

	static alignas(4) CanMramSidfe can0_rx_standard_filter;
	static alignas(4) CanMramRxbe  can0_rx_buffer;
	static alignas(4) CanMramTxbe  can0_tx_buffer;
	static alignas(4) CanMramTxefe can0_tx_event_fifo;
	static alignas(4) CanMramSidfe can1_rx_standard_filter;
	static alignas(4) CanMramRxbe  can1_rx_buffer;
	static alignas(4) CanMramTxbe  can1_tx_buffer;
	static alignas(4) CanMramTxefe can1_tx_event_fifo;

	static const uint8_t dlc_to_size[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64 };

	void InitCAN0();
	void InitCAN1();

	void can0_com(void);
	void can1_com(void);
	void can0_transmit(uint8_t size, uint8_t* data);

#ifdef __cplusplus
}
#endif

#endif //__CANLIB_H__
