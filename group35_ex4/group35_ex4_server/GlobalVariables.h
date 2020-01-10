#pragma once
#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__


#include "HardCodedData.h"
#include "GlobalVariables.h"

extern HANDLE ThreadHandles[MAX_NUM_CLIENTS];
extern ServiceThreadParams ThreadInputs[MAX_NUM_CLIENTS];

// find opponent barirer semaphre
extern HANDLE find_opp_sem;
extern HANDLE find_opp_mutex;
extern int barrier_count;

// file mutex
extern HANDLE com_file_mutex ;

extern HANDLE com_sem[MAX_NUM_CLIENTS];

#endif // __GLOBALVARIABLES_H__

