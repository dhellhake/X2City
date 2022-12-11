/* 
* ADS1118.h
*
* Created: 18.07.2022 22:14:39
* Author: dominik hellhake
*/
#ifndef __ADS1118_H__
#define __ADS1118_H__

#include "sam.h"
#include "..\..\Task.h"

enum class ADS1118_PORT
{
	PORT_0 = 0x00,
	PORT_1 = 0x01,
	PORT_2 = 0x02,
	PORT_3 = 0x03,
	PORT_4 = 0x04,
	PORT_5 = 0x05,
	PORT_6 = 0x06,
	PORT_7 = 0x07	
};


class ADS1118 : public Task
{
	/************************************************************************/
	/* Executable Interface implementation                                  */
	/************************************************************************/
	virtual RUN_RESULT Run(uint32_t timeStamp);

	/************************************************************************/
	/* Class implementation                                                 */
	/************************************************************************/
	private:
		ADS1118_PORT CurrentPort = ADS1118_PORT::PORT_0;
		
		uint8_t Config[8 * 2] = {
			 0b01101010, 0b01000010, // A0 on ADS0 => Set Sample to A0
			 0b01101010, 0b01011010, // A0 on ADS1 => Set Sample to A1
			 0b01101010, 0b01101010, // A1 on ADS1 => Set Sample to A2
			 0b01101010, 0b01111010, // A2 on ADS1 => Set Sample to A3
			 0b01101010, 0b01001010, // A3 on ADS1 => Set Sample to A0
		};

		int16_t ReadPortAndSetConfig(uint8_t* config, uint8_t adsIdx);
	
}; //ADS1118

extern ADS1118 ADS;

#endif //__ADS1118_H__
