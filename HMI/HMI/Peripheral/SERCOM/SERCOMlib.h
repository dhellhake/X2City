/* 
* SERCOMlib.h
*
* Created: 20.09.2020 13:55:34
* Author: dominik hellhake
*/
#ifndef __SERCOMLIB_H__
#define __SERCOMLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

	#include "samc21.h"
	
	void InitSERCOM1();
	uint8_t SERCOM1_I2CStart(uint8_t address);
	uint8_t SERCOM1_I2CSendByte(uint8_t data);
	uint8_t SERCOM1_I2CStop();
	
#ifdef __cplusplus
}
#endif

#endif //__SERCOMLIB_H__
