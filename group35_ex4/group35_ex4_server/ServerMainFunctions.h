#pragma once
#ifndef __SERVERMAINFUNCTIONS_H__
#define __SERVERMAINFUNCTIONS_H__


#include "HardCodedData.h"
#include "ServerThreads.h"
#include "GlobalVariables.h"

/*
* this is the main function of the server, constantly check for clients and open service thread for each one
* if there is open slot for them.
* Return :
*	return ERROR_CODE if error accrued, and 0 else
*/
int serverMain();

/*
* This function search for an open slot in the global thread array
* 
* Return :
*	return ERROR_CODE if error accrued, and the index of the slot if found. if not found retuen the Max 
*/
static int FindFirstUnusedThreadSlot();

/*
* This function intialize a socket and bind it to a port and listening on it.
* Arguments:
*			acceptParams - pointer to an  AcceptSocketParamsstrut to be update and initialized.
* Return :
*	return ERROR_CODE if error accrued
*/

int initializeListeningSocket(AcceptSocketParams *acceptParams);

/*
* This function initialize all global semaphores and mutexes.
*
* Return :
*	return ERROR_CODE if error accrued 
*/
int initializeSemaphores();

/*
* This function closes all global semaphore and mutexes
*/
void closeSemaphores();

/*
* IntializeLeaderboard intialize the global struct that hold the leaderboard
* data, to the saved file of the leaderboard that saved on the server.
* Input Arguments:
*	first_p - a pointer to the pointer of the first item on the struct list.
* Output:
*	the function will return -1 if failed.
*/
int IntializeLeaderboard(leaderboard_player **first_p);

/*
* FreeLeaderboard gets the leaderboard list and free all of its allocated memory.
* Input Arguments:
*	first_p - a pointer to the pointer of the first item on the struct list.
* Output:
*	the function will return -1 if failed.
*/
int FreeLeaderboard(leaderboard_player **first_p);

/*
* ReplaceCommaStr will get the line that read from leaderboard.csv and replace all the
* commas with space for sscanf
* Input Argument:
*	line_str - an str that holds the line read from the csv
*/
void ReplaceCommaStr(char *line_str);

/*
* sends server denied massage to the client 
* commas with space for sscanf
* Input Argument:
*	acceptSocket	- the socket to the client beeing denied.
* Return:
*		return -1 if error accured, 0 else.
*/
int sendServerDenied(SOCKET acceptSocket);
#endif // __SERVERMAINFUNCTIONS_H__