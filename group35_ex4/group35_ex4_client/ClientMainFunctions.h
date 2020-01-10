#pragma once
#ifndef __CLIENTMAINFUNCTIONS_H__
#define __CLENTMAINFUNCTIONS_H__

#include "ThreadFunctions.h"
#include "SharedHardCodedData.h"


/*
* this is the main function of the client
* Return :
*	return ERROR_CODE if error accrued, and 0 else
*/
int clientMain();

/*
* This function initilize the client socket and bind it to a port. than try to connect to the server.
* Arguments:
*	sock - pointer to asocket to be updated as a connected socket to the server
* Return:
*	the funciton return ERROR_CODE if coudent connect or error accrued 

*/
int initializeConnection(SOCKET *sock);

#endif // __CLIENTMAINFUNCTIONS_H__