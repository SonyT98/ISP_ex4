#pragma once
#ifndef __CLIENTMAINFUNCTIONS_H__
#define __CLENTMAINFUNCTIONS_H__

#include "ThreadFunctions.h"
#include "SharedHardCodedData.h"
#include "HardCodedData.h"


/*
* this is the main function of the client.
* Input Arguments:
*	username - a string of the client username.
*	server_adr - the server address as a string.
*	server_port - the server port as a string.
* Return :
*	return ERROR_CODE if error accrued, and 0 else.
*/
int clientMain(char *username, char *server_adr,char *server_port);

/*
* This function initilize the client socket and bind it to a port. than try to connect to the server.
* In addition, it update the client about the connection and return if the user want to connect again.
* Input Arguments:
*	sock - pointer to asocket to be updated as a connected socket to the server.
*	server_adr - the server address as a string.
*	server_port - the server port as a string.
*	connect_again - a int that is 1 if the client want to connect again (if the connection failed).
* Return:
*	the funciton return ERROR_CODE if coudent connect or error accrued. 
*/
int initializeConnection(SOCKET *sock, char *server_adr, char *server_port, int *connect_again);

/*
* ConnectionErrorMenu prints to the user the connection error menu, which different for every type.
* Input Arguments:
*	user_pick - the user pick from the menu. 1 is connect again and 2 is disconnect.
*	connection_error_type - get the connection error type to send to the client.
*	server_adr - the server address as a string.
*	server_port - the server port as a string.
* Return :
*	return ERROR_CODE if error accrued, and 0 else.
*/
int ConnectionErrorMenu(int *user_pick, int connection_error_type, char *server_adr, char *server_port);

#endif // __CLIENTMAINFUNCTIONS_H__