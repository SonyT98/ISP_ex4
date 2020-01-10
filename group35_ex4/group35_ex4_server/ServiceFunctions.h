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
*	replay - an index to the server function to replay the game.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int CPUGame(SOCKET sock, int *replay);


/*
* Message_cut cutts the received message and to its message type
* and message information as strings.
* Input Arguments:
*	message - the whole message the we recevied.
*	max_size - the message size.
*	message_type - the message type that we want to update;
*	info - the information message we want to update;
* Output:
*	return -1 if the message isn't right.
*/
int MessageCut(char *message, int message_size, char* message_type, char *info);

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