#include "GlobalVariables.h"


HANDLE ThreadHandles[MAX_NUM_CLIENTS] = { NULL };
ServiceThreadParams ThreadInputs[MAX_NUM_CLIENTS];

// find opponent barrier
HANDLE find_opp_sem = NULL;
HANDLE find_opp_mutex = NULL;
int barrier_count = 0;

// file mutex
HANDLE com_file_mutex = NULL;

HANDLE com_sem[MAX_NUM_CLIENTS] = { NULL };
