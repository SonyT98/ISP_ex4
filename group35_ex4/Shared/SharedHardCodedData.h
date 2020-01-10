#pragma once
#ifndef __SHAREDHARDCODEDDATA_H__
#define __SHAREDHARDCODEDDATA_H___


#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>

#define ERROR_CODE ((int)(-1))
#define MAX_USERNAME 20
#define MAX_MESSAGE 60
#define SENDRECV_WAITTIME 15000
#define USER_WAITTIME 600000

#define SERVER_ADDRESS_STR "127.0.0.1"
#define SERVER_PORT 8888
#define CONNECTION_DISCONNECTED 1

/*------------------------- Client Messages ----------------------------------*/

#define CLIENT_REQUEST "CLIENT_REQUEST"
#define CLIENT_MAIN_MENU "CLIENT_MAIN_MENU"
#define CLIENT_CPU "CLIENT_CPU"
#define CLIENT_VERSUS "CLIENT_VERSUS"
#define CLIENT_LEADERBOARD "CLIENT_LEADERBOARD"
#define CLIENT_PLAYER_MOVE "CLIENT_PLAYER_MOVE"
#define CLIENT_REPLAY "CLIENT_REPLAY"
#define CLIENT_REFRESH "CLIENT_REFRESH"
#define CLIENT_DISCONNECT "CLIENT_DISCONNECT"

/*------------------------- Server Messages ----------------------------------*/

#define SERVER_MAIN_MENU "SERVER_MAIN_MENU"
#define SERVER_APPROVED "SERVER_APPROVED"
#define SERVER_DENIED "SERVER_DENIED"
#define SERVER_INVITE "SERVER_INVITE"
#define SERVER_PLAYER_MOVE_REQUEST "SERVER_PLAYER_MOVE_REQUEST"
#define SERVER_GAME_RESULTS "SERVER_GAME_RESULTS"
#define SERVER_GAME_OVER_MENU "SERVER_GAME_OVER_MENU"
#define SERVER_OPPONENT_QUIT "SERVER_OPPONENT_QUIT"
#define SERVER_NO_OPPONENTS "SERVER_NO_OPPONENTS"
#define SERVER_LEADERBOARD "SERVER_LEADERBOARD"
#define SERVER_LEADERBOARD_MENU "SERVER_LEADERBOARD_MENU"

typedef struct
{
	SOCKET sock;
	char *array_t;
	int array_size;

} sendthread_s;

#define STRINGS_ARE_EQUAL( Str1, Str2 ) ( strcmp( (Str1), (Str2) ) == 0 )

#endif // __HARDCODEDDATA_H___