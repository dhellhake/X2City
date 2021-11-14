/*
 * Executable.h
 *
 * Created: 08.07.2018 09:44:01
 *  Author: Dominik Hellhake
 */
#ifndef EXECUTABLE_H_
#define EXECUTABLE_H_

#include "sam.h"

enum class RUN_RESULT
{
	SUCCESS,
	IDLE,
	ERROR,
	NOT_IMPLEMENTED
};

class Executable
{
	public:	
		virtual RUN_RESULT Run(uint32_t timeStamp) { return RUN_RESULT::NOT_IMPLEMENTED; };
			
}; //Executable

#endif /* EXECUTABLE_H_ */