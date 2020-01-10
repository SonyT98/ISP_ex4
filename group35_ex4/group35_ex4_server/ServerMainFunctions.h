#pragma once
#ifndef __SERVERMAINFUNCTIONS_H__
#define __SERVERMAINFUNCTIONS_H__


#include "HardCodedData.h"
#include "SharedHardCodedData.h"
#include "ServerThreads.h"

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


#endif // __SERVERMAINFUNCTIONS_H__