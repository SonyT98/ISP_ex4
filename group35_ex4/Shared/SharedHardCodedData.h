#pragma once
#ifndef __SHAREDHARDCODEDDATA_H__
#define __SHAREDHARDCODEDDATA_H___


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

#define SERVER_ADDRESS_STR "127.0.0.1"
#define SERVER_PORT 8888

/*------------------------- Client Messages ----------------------------------*/

#define CLIENT_REQUEST "CLIENT_REQUEST:%s\n"
#define CLIENT_MAIN_MENU "CLIENT_MAIN_MENU:\n"
#define CLIENT_CPU "CLIENT_CPU:\n"
#define CLIENT_VERSUS "CLIENT_VERSUS:\n"
#define CLIENT_LEADERBOARD "CLIENT_LEADERBOARD:\n"
#define CLIENT_PLAYER_MOVE "CLIENT_PLAYER_MOVE:%s\n"
#define CLIENT_REPLAY "CLIENT_REPLAY:\n"
#define CLIENT_REFRESH "CLIENT_REFRESH:\n"
#define CLIENT_DISCONNECT "CLIENT_DISCONNECT:\n"

/*------------------------- Server Messages ----------------------------------*/

#define SERVER_MAIN_MENU "SERVER_MAIN_MENU:\n"
#define SERVER_APPROVED "SERVER_APPROVED:\n"
#define SERVER_DENIED "SERVER_DENIED:%s\n"
#define SERVER_INVITE "SERVER_INVITE:%s\n"
#define SERVER_PLAYER_MOVE_REQUEST "SERVER_MAIN_MENU:\n"
#define SERVER_GAME_RESULTS "SERVER_GAME_RESULTS:%s;%s;%s;%s\n"
#define SERVER_GAME_OVER_MENU "SERVER_GAME_OVER_MENU:\n"
#define SERVER_OPPONENT_QUIT "SERVER_OPPONENT_QUIT:%s\n"
#define SERVER_NO_OPPONENTS "SERVER_NO_OPPONENTS:\n"
#define SERVER_LEADERBOARD "SERVER_LEADERBOARD:"
#define SERVER_LEADERBOARD_MENU "SERVER_LEADERBOARD_MENU:\n"

typedef struct
{
	SOCKET sock;
	char *send_array;
	int array_size;

} sendthread_s;

#define STRINGS_ARE_EQUAL( Str1, Str2 ) ( strcmp( (Str1), (Str2) ) == 0 )

#endif // __HARDCODEDDATA_H___