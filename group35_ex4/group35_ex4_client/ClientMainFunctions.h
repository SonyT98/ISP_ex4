#pragma once
#ifndef __CLIENTMAINFUNCTIONS_H__
#define __CLENTMAINFUNCTIONS_H__

#include "ThreadFunctions.h"
#include "SharedHardCodedData.h"
#include "HardCodedData.h"
#include "UserInterfaceFunctions.h"


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
* Return:
*	return ERROR_CODE if error accrued, connection failed code, zero if succeed.
*/
int initializeConnection(SOCKET *sock, char *server_adr, char *server_port);

/*
* Request connection from the server and connect if allowed
* Input Arguments:
*	sock - socket to be updated as a connected socket to the server.
*	server_port - the server port as a string.
*	server_adr - the server address as a string.
*	username - a string of the client username.
* Return :
*	return ERROR_CODE if error accrued, connection failed code, zero if succeed.
*/
int RequestConnection(SOCKET sock, char *username);

/*
* RecieveMessageFromServer receive the messages from the server and act accordingly
*	sock - socket to be updated as a connected socket to the server.
*	connect_again - a int that is 1 if the client want to connect again (if the connection failed).
*	server_port - the server port as a string.
*	server_adr - the server address as a string.
*	main_menu_selection - the main menu selection of the message.
* Return :
*	return ERROR_CODE if error accrued, connection failed code, zero if succeed.
*/
int ReceiveMessageFromServer(SOCKET sock, int *main_menu_selection);


#endif // __CLIENTMAINFUNCTIONS_H__