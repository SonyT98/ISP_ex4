#pragma once
#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__


#include "HardCodedData.h"
#include "GlobalVariables.h"

extern HANDLE ThreadHandles[MAX_NUM_CLIENTS];
extern ServiceThreadParams ThreadInputs[MAX_NUM_CLIENTS];

//Leaderboard struct variables
leaderboard_player *first_player;

// find opponent barirer semaphre
extern HANDLE find_opp_event;
extern HANDLE find_opp_mutex;
extern int barrier_count;
extern int gameon;
// file mutex
extern HANDLE com_file_mutex ;

extern HANDLE com_sem[MAX_NUM_CLIENTS];

#endif // __GLOBALVARIABLES_H__

