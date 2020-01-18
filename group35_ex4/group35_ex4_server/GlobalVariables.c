#include "GlobalVariables.h"


HANDLE ThreadHandles[MAX_NUM_CLIENTS] = { NULL };
ServiceThreadParams ThreadInputs[MAX_NUM_CLIENTS];

char usernames[MAX_NUM_CLIENTS][MAX_USERNAME] = {"", ""};



//Leaderboard struct variables
leaderboard_player *first_player = NULL;

// find opponent barrier
HANDLE find_opp_event = NULL;
HANDLE find_opp_mutex = NULL;
int barrier_count = 0;
int gameon = 0;

// file mutex
HANDLE com_file_mutex = NULL;

HANDLE com_sem[MAX_NUM_CLIENTS] = { NULL };

extern HANDLE username_mutex = NULL;

// Versus game replay flag
int replay_arr[MAX_NUM_CLIENTS];
