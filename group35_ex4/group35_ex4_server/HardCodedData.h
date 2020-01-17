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

#define WRITE_TO_GAMESESSION 0
#define READ_FROM_GAMESESSION 1

#define PLAYER_WON 1
#define PLAYER_LOST 2
#define PLAYER_TIE 0

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

typedef struct
{
	char username[MAX_USERNAME + 1];
	int won;
	int lost;
	double ratio;
	struct leaderboard_player *next;

} leaderboard_player;

#endif // __HARDCODEDDATA_H___
