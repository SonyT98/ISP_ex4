#pragma once
#ifndef __SERVICEFUNCTIONS_H__
#define __SERVICEFUNCTIONS_H__


#include "HardCodedData.h"
#include "SharedHardCodedData.h"
#include "ServerThreads.h"
#include "SocketSendRecvTools.h"
#include "ThreadFunctions.h"
#include "GlobalVariables.h"

/*
* ClientUsername gets the client username through sending and reciving the
* required messages.
* Input Arguments:
*	sock - the socket we opend with the client.
*	username - a string of the username of the client.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int ClientUsername(SOCKET sock, char *username);

/*
* SelectFromMenu gets the client menu selectio  through sending and reciving the
* required messages.
* Input Arguments:
*	sock - the socket we opend with the client.
*	menu_select - the menu selection of the client.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int SelectFromMenu(SOCKET sock, int *menu_select);

/*
* CPUGame set the game of the client against the cpu.
* Input Arguments:
*	sock   - the socket we opend with the client.
*	username - a string of the username of the client.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int CPUGame(SOCKET sock,char *username);

/*
* VersusGame set the game of the client against the versus mode.
* Input Arguments:
*	sock			- the socket we opend with the client.
*	index			- index of the thread used by this client
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int VersusGame(SOCKET sock, int index);


/*
* PlayVersus ask for the client for their move and use gamesassion.txt to transfer the information between
* thetwo threads.
* Input Arguments:
*	sock			- the socket we opend with the client.
*	index			- index of the thread used by this client
*   player_move_s   - the string that include the player move.
*   opp_move_s      - the string that include the cpu move.
*	player_move		- pointer to int to be updated with the opponent move.
*	opponenet_move	- pointer to int to be updated with the opponent move.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int playVersus(SOCKET sock, int index, char* player_move_s, char* opp_move_s, int *player_move, int *opponent_mov)




/*
* exeute a barrier to find if another oppenent wants to play
* Output:
*	return OPPONENT_FOUND			1	- if another opponent logged in
*	return OPPONENT_WASENT_FOUND	2   - if no opponent has logged
*	return ERROR_CODE			   -1	- if an error accrued
*/
int findOpponentBarrier();

/*
* check if the file exits if not create a blank copy of it
* Output:
*	return 	0	- if opened the file.
*	return 	1   - if the file already been created.
*	return -1	- if an error accrued.
*/
int MakeSureFileExist();

/*
	check if file exits
	Arguments:
		filename - string consist the filename.
	Return:
		TRUE:	file exist
		FALSE:	file doesn't exist
*/
bool FileExists(const TCHAR *fileName);




/*
* EndGameStatus gets the player and the second player username and moves, and send the data to the client.
* In addition update replay if the client want to replay his game.
* Input Arguments:
*	sock   - the socket we opend with the client.
*	username - the player username.
*	other_player - the username of the second player.
*	my_move - string of the client move.
*	other_move - string of the other player move.
*	winning_player - the winning player: 1 is the client, 2 is the other player, 0 is a tie.
*	replay - if the client want to play again.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int EndGameStatus(SOCKET sock, char *username, char *other_player, char *my_move, 
	char *other_move, int winning_player, int *replay);

/*
* GetMoveFromClient send to the client to pick the move he wants, and return his move.
* Input Arguments:
*	sock   - the socket we opend with the client.
*	player_move_s - the string that include the player move.
*	player_move - the move number of the client.
* Output:
*	return -1 if failed, otherwise return the exit code of the thread recv and send.
*/
int GetMoveFromClient(SOCKET sock, char* player_move_s, int *player_move);

/*
	This function read and write the GameSession.txt file the client move. depend on the inputs.
	Arguments:
		player_move_s	- string consist the player move. if in read mode will be update to the opponent move
		player_move_i	- int consist the player move. if in read mode will be update to the opponent move
		read_write		- int that indicate whether read or write to the .txt file. use WRITE_TO_GAMESESSION and READ_FROM_GAMESESSION.
	Return:
		-1 if Error, 0 else.
*/
int ReadOrWriteToGameSassionFile(char* player_move_s, int* player_move_i, int read_write);

/*
	This function check if the client want to play with the opponent again if he does the function check if the opponent want to play too.
	Arguments:
			replay_choice	- 1 if the client wants to play again. 0 if client wants to gotomain menu
			index			- index of the client thread.
			sock			- the socket of the client connection.
	Return:
*		return OPPONENT_STAYED_GAME		1	- if the opponent wants to play again
*		return OPPONENT_QUIT_GAME		2   - if the opponent doesn't want to play again
*		return ERROR_CODE			   -1	- if an error accrued
*/
int VersusReplayOptionCheck(SOCKET sock, int replay_choice, int index);


/*
* PlayMatch is a function that gets two moves of two players and return the index of the
* winning player. if there was a tie the function return 0.
* Input Arguments:
*	player1_move - the move of the first player.
*	player2_move - the move of the second player.
* Output:
*	return 1 or 2 if the first or second player won accordinly. return 0 if there
*	is a tie.
*/
int PlayMatch(int player1_move, int player2_move);

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
#endif // __SERVICEFUNCTIONS_H__