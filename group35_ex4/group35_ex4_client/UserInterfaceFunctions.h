// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 4 in the Course Introduction to system programming
// This file include all the main functions of the client

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
* MainMenuSelection gets the msin menu selection from the client and send to the server.
* Input Arguments:
*	sock - socket to be updated as a connected socket to the server.
*	connect_again - a int that is 1 if the client want to connect again (if the connection failed).
*	server_port - the server port as a string.
*	server_adr - the server address as a string.
*	main_menu_selection - the main menu selection of the message.
* Return :
*	return ERROR_CODE if error accrued, connection failed code, zero if succeed.
*/
int MainMenuSelection(SOCKET sock, int *main_menu_selection);


/*
* PlayerMoveRequest gets the player move from the client and send it to the server
* Input Arguments:
*	sock - socket to be updated as a connected socket to the server.
* Return :
*	return ERROR_CODE if error accrued, connection failed code, zero if succeed.
*/
int PlayerMoveRequest(SOCKET sock);

/*
* StirngUpper gets a string and change the characters in the string to upper case.
* Input Arguments:
*	a - the string we want to change.
*/
void StringUpper(char *a);

/*
* GameResultDisplay will display the end of match result for the client.
* Input Arguments:
*	message_info - the match information the server send to the client.
* Return :
*	return ERROR_CODE if error accrued, connection failed code, zero if succeed.
*/
int GameResultDisplay(char *match_info);

/*
*	GameOverMenu shows the game over menu to the user and wait for response then send it to the server.
* Input Arguments:
*	sock - socket to be updated as a connected socket to the server.
* Output:
*	return ERROR_CODE if failed.
*/
int GameOverMenu(SOCKET sock);

/*
* LeaderboardDisplay gets the leaderboard message and display it to the user screen.
* Input Arguments:
*	leaderboard - the leaderboard message.
* Output:
*	return ERROR_CODE if failed.
*/
int LeaderboardDisplay(char *leaderboard);

/*
* ReplaceCommaWithDoubleTab gets a string with multiple commas inside, and replace it with two tabs.
* Input Arguments:
*	str - the string message.
*	str_new - the new string message.
* Output:
*	return ERROR_CODE if failed.
*/
int ReplaceCommaWithDoubleTab(char *str, char **str_new);

/*
* LeaderboardMenu diplay to the user the leaderboard display and send his selection to the server
* Input Arguments:
*	sock - the connection socket
* Output:
*	return ERROR_CODE if error accrued, connection failed code, zero if succeed.
*/
int LeaderboardMenu(SOCKET sock);
#endif // __USERINTERFACEFUNCTIONS_H__