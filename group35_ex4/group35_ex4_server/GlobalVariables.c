#include "GlobalVariables.h"


HANDLE ThreadHandles[MAX_NUM_CLINTS];
ServiceThreadParams ThreadInputs[MAX_NUM_CLINTS];

// find opponent barrier
HANDLE find_opp_sem = NULL;
HANDLE find_opp_mutex = NULL;
int barrier_count = 0;
