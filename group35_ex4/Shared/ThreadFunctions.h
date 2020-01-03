
#ifndef __THREADFUNCTIONS_H__
#define __THREADFUNCTIONS_H___

// Includes --------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>

// Function Declarations -------------------------------------------------------

/*
* A simplified API for creating threads.
* Input Arguments:
*   p_start_routine: A pointer to the function to be executed by the thread.
*     This pointer represents the starting address of the thread.
*     The function has to have this specific signature:
*       DWORD WINAPI FunctionName(LPVOID lpParam);
*     With FunctionName being replaced with the function's name.
*     arg_pointer: a pointer to the argument of the thread.
* Output Arguments:
*   p_thread_id: A pointer to a variable that receives the thread identifier.
*     If this parameter is NULL, the thread identifier is not returned.
* Return:
*   If the function succeeds, the return value is a handle to the new thread.
*   If the function fails, the return value is NULL.
*   To get extended error information, call GetLastError.
* Notes:
*   This function is just a wrapper for CreateThread.
*/
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id,
	void *arg_pointer);

/*
* Close Thread handles from thread array in places 0 - last_thread.
* Input Arguments :
*	p_thread_handles:	array of the threads handles
*	last_thread:		int of the place to the last thread to be closed
* Return :
*	return -1 if the thread wont close.
*/
int CloseThreads(HANDLE* p_thread_handles,
	int last_thread);


#endif // __THREADFUNCTIONS_H___