#pragma once
#ifndef __SERVERMAINFUNCTIONS_H__
#define __SERVERMAINFUNCTIONS_H__


#include "HardCodedData.h"
#include "SharedHardCodedData.h"
#include "ServerThreads.h"


int serverMain();

static int FindFirstUnusedThreadSlot();

int initializeListeningSocket(AcceptSocketParams *acceptParams);
#endif // __SERVERMAINFUNCTIONS_H__