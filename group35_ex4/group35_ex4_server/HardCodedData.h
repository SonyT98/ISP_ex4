#pragma once
#ifndef __HARDCODEDDATA_H__
#define __HARDCODEDDATA_H___

#define MAX_NUM_CLINTS 2

#define ROCK 1
#define PAPER 2
#define SCISSORS 3
#define LIZARD 4
#define SPOCK 5

#include "SharedHardCodedData.h"

typedef struct
{
	SOCKET mainSocket;
	SOCKET AcceptSocket;

}AcceptSocketParams;

typedef struct
{
	SOCKET	client_socket;
	int		index;

}ServiceThreadParams;



#endif // __HARDCODEDDATA_H___
