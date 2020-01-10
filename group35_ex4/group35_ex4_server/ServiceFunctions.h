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
* ActivateThread gets a null handle for the thread and activate the specific thread
* for sending of receving a message.
* Input Arguments:
*	arg			  - an argument for the thread function.
*	recv_or_send  - zero if we recevie and 1 if we send.
*	waittime	  - the wait time of the thread.
* Output:
*	return -1 if failed, or the exit code of the thread otherwise.
*/
int ActivateThread(void *arg, int recv_or_send, int waittime);
#endif // __SERVICEFUNCTIONS_H__