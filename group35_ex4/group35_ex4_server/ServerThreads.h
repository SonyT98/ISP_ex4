#pragma once
#ifndef __SERVERTHREADS_H__
#define __SERVERTHREADS_H__

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
* GetClientName communicate with the client and gets his username
* for the start of the game.
* Input Arguments:
*	username - a string that will be updated when the client sends
*	his username.
* Outputs:
*	return -1 if there is an error.
*/
int GetClientName(char *username);

#endif // __SERVERTHREADS_H__

