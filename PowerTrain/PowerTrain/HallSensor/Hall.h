/*
 * Hall.h
 *
 * Created: 21.09.2021 20:29:30
 *  Author: dominik hellhake
 */ 
#ifndef HALL_H_
#define HALL_H_

#include "samc21.h"

enum class HALL_STATE
{	//State decoding: 0vUVW
	HALL_STATE_5 = 0b001,	// 1
	HALL_STATE_6 = 0b011,	// 3
	HALL_STATE_1 = 0b010,	// 2
	HALL_STATE_2 = 0b110,	// 6
	HALL_STATE_3 = 0b100,	// 4
	HALL_STATE_4 = 0b101,	// 5
	UNDEFINED_1  = 0b000,
	UNDEFINED_2  = 0b111
};

typedef enum HallSignal
{
	HallSignalU = 0x01,
	HallSignalV = 0x02,
	HallSignalW = 0x04
} HallSignal;

typedef enum DrvDir
{
	DrvDir_Undefined =					0xFF,
	DrvDir_Forward	=					0x01,
	DrvDir_Forward_Brake =				0x02,
	DrvDir_Reverse =					0x03,
	DrvDir_Reverse_Brake =				0x04
} DrvDir;


#endif /* HALL_H_ */