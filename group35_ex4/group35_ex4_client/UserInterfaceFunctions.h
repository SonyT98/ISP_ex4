#pragma once
#ifndef __USERINTERFACEFUNCTIONS_H__
#define __USERINTERFACEFUNCTIONS_H__

#include "ThreadFunctions.h"
#include "SharedHardCodedData.h"
#include "HardCodedData.h"
#include "ClientMainFunctions.h"

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

/*
* MainMenuSelection gets the msin menu selection from the client and send to the server
*	sock - socket to be updated as a connected socket to the server.
*	connect_again - a int that is 1 if the client want to connect again (if the connection failed).
*	server_port - the server port as a string.
*	server_adr - the server address as a string.
*	client_disconnected - will update to 1 if the client want to disconnect.
* Return :
*	return ERROR_CODE if error accrued, connection failed code, zero if succeed.
*/
int MainMenuSelection(SOCKET sock, int *connect_again, char *server_adr, char *server_port, int *client_disconnected);

#endif // __USERINTERFACEFUNCTIONS_H__