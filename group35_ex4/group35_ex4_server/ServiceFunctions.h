#pragma once
#ifndef __SERVICEFUNCTIONS_H__
#define __SERVICEFUNCTIONS_H__


#include "HardCodedData.h"
#include "SharedHardCodedData.h"
#include "ServerThreads.h"
#include "SocketSendRecvTools.h"
#include "ThreadFunctions.h"

/*
* ClientUsername gets the client username through sending and reciving the
* required messages.
* Input Arguments:
*	sock - the socket we opend with the client.
*	username - a string of the username of the client.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int ClientUsername(SOCKET sock, char *username);

/*
* SelectFromMenu gets the client menu selectio  through sending and reciving the
* required messages.
* Input Arguments:
*	sock - the socket we opend with the client.
*	menu_select - the menu selection of the client.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int SelectFromMenu(SOCKET sock, int *menu_select);

/*
* CPUGame set the game of the client against the cpu.
* Input Arguments:
*	sock   - the socket we opend with the client.
*	player_move_s - the string that include the player move.
*	cpu_move_s - the string that include the cpu move.
*	winning_player - the winning player: 1 is the player, 2 is the cpu, 0 is a tie.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int CPUGame(SOCKET sock, char* player_move_s, char* cpu_move_s, int *winning_player);

/*
* VersusGame set the game of the client against the versus mode.
* Input Arguments:
*	sock   - the socket we opend with the client.
*	player_move_s - the string that include the player move.
*	opp_move_s - the string that include the cpu move.
*	winning_player - the winning player: 1 is the player, 2 is the cpu, 0 is a tie.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int VersusGame(SOCKET sock, char* player_move_s, char* opp_move_s, int *winning_player)

/*
* EndGameStatus gets the player and the second player username and moves, and send the data to the client.
* In addition update replay if the client want to replay his game.
* Input Arguments:
*	sock   - the socket we opend with the client.
*	username - the player username.
*	other_player - the username of the second player.
*	my_move - string of the client move.
*	other_move - string of the other player move.
*	winning_player - the winning player: 1 is the client, 2 is the other player, 0 is a tie.
*	replay - if the client want to play again.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int EndGameStatus(SOCKET sock, char *username, char *other_player, char *my_move, 
	char *other_move, int winning_player, int *replay);

/*
* PlayMatch is a function that gets two moves of two players and return the index of the
* winning player. if there was a tie the function return 0.
* Input Arguments:
*	player1_move - the move of the first player.
*	player2_move - the move of the second player.
* Output:
*	return 1 or 2 if the first or second player won accordinly. return 0 if there
*	is a tie.
*/
int PlayMatch(int player1_move, int player2_move);
#endif // __SERVICEFUNCTIONS_H__