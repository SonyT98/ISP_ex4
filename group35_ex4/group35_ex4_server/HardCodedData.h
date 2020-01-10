#pragma once
#ifndef __HARDCODEDDATA_H__
#define __HARDCODEDDATA_H___

#define MAX_NUM_CLIENTS 2

#define ROCK 1
#define PAPER 2
#define SCISSORS 3
#define LIZARD 4
#define SPOCK 5

#define WAIT_FOR_OPP_TIME 30000

#define OPPONENT_FOUND  1
#define OPPONENT_WASENT_FOUND 2 

#include "SharedHardCodedData.h"
#include "GlobalVariables.h"
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
