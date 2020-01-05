#pragma once
#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__


#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define MAX_NUM_OF_CLIENTS 2


extern HANDLE ThreadHandles[MAX_NUM_OF_CLIENTS];
extern SOCKET ThreadInputs[MAX_NUM_OF_CLIENTS];


#endif // __GLOBALVARIABLES_H__

