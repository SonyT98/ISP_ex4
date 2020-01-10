#pragma once
#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__


#include "HardCodedData.h"


extern HANDLE ThreadHandles[MAX_NUM_CLIENTS];
ServiceThreadParams ThreadInputs[MAX_NUM_CLIENTS];

// find opponent barirer semaphre
HANDLE find_opp_sem;
HANDLE find_opp_mutex;
int barrier_count;

#endif // __GLOBALVARIABLES_H__

