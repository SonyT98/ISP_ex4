// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 4 in the Course Introduction to system programming
// This is the functions of the leaderboard

#pragma once
#ifndef __LEADERBOARDFUNCTIONS_H__
#define __LEADERBOARDFUNCTIONS_H__


#include "HardCodedData.h"
#include "SharedHardCodedData.h"
#include "ServerThreads.h"
#include "SocketSendRecvTools.h"
#include "ThreadFunctions.h"
#include "GlobalVariables.h"


/*
* RemovePlayerFromList will remove a specific player from the list.
* Input Argument:
*	before_player - a pointer to the player before the specific player.
*	player - a pointer to the specific player.
*	first - a pointer to the first item in the list.
*/
void RemovePlayerFromList(leaderboard_player **before_player, leaderboard_player **player, leaderboard_player **first);

/*
* SearchInList will search if there is existing username in list and will remove this item from it.
* If there isn't an item in the list, the function will create one.
* Input Arguments:
*	current_player	- a pointer that will hold the username player (after been removed from the list).
*	username		- a string of the player username.
*	gamestat		- the game result of the username. 1 if he won, 2 if he lost, and 0 if there was a tie.
*	first_p	- the first player on the struct list.
* Output:
*	the function will return -1 if failed.
*/
int SearchInList(leaderboard_player **current_player, char *username, int gamestat, leaderboard_player **first_p);

/*
* InsertPlayer gets the list and the player we want to insert, and the function insert the player to the currect place.
* Input Arguments:
*	first_p - a pointer to the first player.
*	current_p - a pointer to the current player.
* Output:
*	the function will return -1 if failed.
*/
int InsertPlayer(leaderboard_player** current_player, leaderboard_player **first_p);

/*
* WriteToFile gets the list and write the struct list to the csv file.
* Input Arguments:
*	first_p - a pointer to the first player.
* Output:
*	the function will return -1 if failed.
*/
int WriteToFile(leaderboard_player** first_p);

/*
* UpdateLeaderboard gets the username of the player and update the leaderboard struct and file
* Input Argument:
*	username- a string of the player username.
*	gamestat- the game result of the username. 1 if he won, 2 if he lost, and 0 if there was a tie.
* Output:
*	the function will return -1 if failed.
*/
int UpdateLeaderboard(char *username, int gamestat);

/*
* LeaderboardMessageSend gets the leaderboard message from the csv file and puts it on packet
* Input Arguments:
*	leaderboard_message - the leaderboard message that we read from the csv.
*	leaderboard_length - the message length.
* Output:
*	the function will return -1 if failed.
*/
int LeaderboardRead(char **leaderboard_message, int *leaderboard_length);

/*
* SendLeaderboardMessage send the leaderboard message, in case its a new leader board.
* The function compare the saved leaderboard with the new one and send it if necessary.
* Input Arguments:
*	sock - the socket that connected to the client.
*	info - the leaderboard new info.
*	info_length - the leaderboard message length.
*	saved - the saved leaderboard.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int SendLeaderboardMessage(SOCKET sock, char **info, int info_length, char **saved);

/*
* LeaderboardSelection send to the client the leaderboard, and the leaderboard menu
* Input Arguments:
*	sock - the socket that connected to the client.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int LeaderBoardSelection(SOCKET sock);

#endif // __LEADERBOARDFUNCTIONS_H__
