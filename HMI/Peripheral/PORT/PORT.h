/*
 * PORT.h
 *
 * Created: 21.05.2023 14:09:34
 *  Author: domin
 */ 
#ifndef PORT_H_
#define PORT_H_

#include "samc21.h"

void InitPORT();

void SetPinPeripheralFunction(uint32_t pinmux);

#endif /* PORT_H_ */