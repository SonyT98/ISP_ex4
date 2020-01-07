#include "GlobalVariables.h"

HANDLE ThreadHandles[MAX_NUM_OF_CLIENTS] = { NULL };
SOCKET ThreadInputs[MAX_NUM_OF_CLIENTS]  = { INVALID_SOCKET };

