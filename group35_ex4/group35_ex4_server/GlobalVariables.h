// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 4 in the Course Introduction to system programming
// This is the global variables

#pragma once
#ifndef __GLOBALVARIABLES_H__
#define __GLOBALVARIABLES_H__


#include "HardCodedData.h"
#include "GlobalVariables.h"

extern HANDLE ThreadHandles[MAX_NUM_CLIENTS];
extern LPWORD ThreadId[MAX_NUM_CLIENTS];
extern ServiceThreadParams ThreadInputs[MAX_NUM_CLIENTS];

extern char usernames[MAX_NUM_CLIENTS][MAX_USERNAME];
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
extern HANDLE com_event[MAX_NUM_CLIENTS];

extern HANDLE username_mutex;

// leaderboard mutex & semaphores
extern HANDLE leaderboard_mutex;
extern HANDLE room_empty;
extern int readers;

extern int replay_arr[MAX_NUM_CLIENTS];

#endif // __GLOBALVARIABLES_H__

