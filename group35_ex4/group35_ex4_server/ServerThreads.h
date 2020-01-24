// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 4 in the Course Introduction to system programming
// This is the threads we operate in the main server

#pragma once
#ifndef __SERVERTHREADS_H__
#define __SERVERTHREADS_H__

// Includes --------------------------------------------------------------------

#include "SharedHardCodedData.h"
#include "HardCodedData.h"
#include "GlobalVariables.h"

// Function Declarations -------------------------------------------------------

DWORD AcceptClientThread(LPSTR lpParam);

/*
* constanly check if user entered the word exit from cmd
* Input Arguments :
	lpParam - can be NULL
* Return :
*	return 2 if exit was enter. -1 if error
*/
DWORD WINAPI CheckExitThread(LPSTR lpParam);

/*
*	This is the main thread that handles each client.
*	lpParam should be type ServiceThreadParams
*/

DWORD WINAPI ServiceThread(LPSTR lpParam);

#endif // __SERVERTHREADS_H__

