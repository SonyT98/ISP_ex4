
#include "ServiceFunctions.h"

int ClientUsername(SOCKET sock, char *username)
{
	//variables
	char message_type[MAX_MESSAGE];
	char message_send[MAX_MESSAGE];

	char *temp_message;

	sendthread_s *packet;

	int size_arr = 0, ret = 0, exit_code = 0, err = 0;

	//malloc for the sendthread_s struct
	packet = (sendthread_s*)malloc(sizeof(sendthread_s));
	if (packet == NULL)
	{
		printf("ERROR: allocate memory for thread arg\n");
		ret = ERROR_CODE;
		goto return_ret;
	}

	//intialize the socket to the thread functions
	packet->sock = sock;


	/*----------------------------recv CLIENT_REQUEST-----------------------------*/
	packet->array_t = NULL;
	packet->array_size = 0;

	//activate the recv thread and get his exit code
	exit_code = ActivateThread((void*)packet, 0, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0){ret = exit_code;  goto cleanup_memory;}

	//temp array the holds the message for the cleanup
	temp_message = packet->array_t;

	//get the message type and the information
	err = MessageCut(packet->array_t, packet->array_size, message_type, username);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto main_cleanup1; }

	//if we didnt got client request message
	if (!STRINGS_ARE_EQUAL(message_type, CLIENT_REQUEST))
	{
		printf("Error: message recieved from the client doesnt match with the protocol\n");
		ret = ERROR_CODE;
		goto main_cleanup1;
	}


	/*----------------------------send SERVER_APPROVED-----------------------------*/
	err = sprintf_s(message_send,MAX_MESSAGE, "%s:\n", SERVER_APPROVED);
	if (err == 0 || err == EOF)
	{
		printf("Error: can't create the message for the client\n");
		ret = ERROR_CODE;
		goto main_cleanup1;
	}
	packet->array_size = strlen(message_send);
	packet->array_t = message_send;

	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto main_cleanup1; }

	ret = exit_code;

main_cleanup1:
	free(temp_message);
cleanup_memory:
	free(packet);
return_ret:
	return ret;
}


int SelectFromMenu(SOCKET sock, int* menu_select)
{
	//variables
	char message_type[MAX_MESSAGE];
	char message_info[MAX_MESSAGE];
	char message_send[MAX_MESSAGE];

	sendthread_s *packet;

	int size_arr = 0, ret = 0, exit_code = 0, err = 0;

	//malloc for the sendthread_s struct
	packet = (sendthread_s*)malloc(sizeof(sendthread_s));
	if (packet == NULL)
	{
		printf("ERROR: allocate memory for thread arg\n");
		ret = ERROR_CODE;
		goto return_ret;
	}

	//intialize the socket to the thread functions
	packet->sock = sock;


	/*----------------------------send SERVER_MAIN_MENU-----------------------------*/
	err = sprintf_s(message_send,MAX_MESSAGE ,"%s:\n", SERVER_MAIN_MENU);
	if (err == 0 || err == EOF)
	{
		printf("Error: can't create the message for the client\n");
		ret = ERROR_CODE;
		goto cleanup_memory;
	}

	packet->array_t = message_send;
	packet->array_size = strlen(message_send);

	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto cleanup_memory; }


	/*--------------------------recv the client request--------------------------*/
	packet->array_t = NULL;
	packet->array_size = 0;

	//activate the recv thread and get his exit code
	exit_code = ActivateThread((void*)packet, 0, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto cleanup_memory; }

	//get the message type and the information
	err = MessageCut(packet->array_t, packet->array_size, message_type, message_info);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto main_cleanup1; }

	//check for the message type to update the menu selection
	if (STRINGS_ARE_EQUAL(message_type, CLIENT_CPU))
		*menu_select = 1;
	else if (STRINGS_ARE_EQUAL(message_type, CLIENT_VERSUS))
		*menu_select = 2;
	else if (STRINGS_ARE_EQUAL(message_type, CLIENT_LEADERBOARD))
		*menu_select = 3;
	else if (STRINGS_ARE_EQUAL(message_type, CLIENT_DISCONNECT))
		*menu_select = 4;
	else
	{
		printf("Error: message recieved from the client doesnt match with the protocol\n");
		ret = ERROR_CODE;
		goto main_cleanup1;
	}



main_cleanup1:
	free(packet->array_t);
cleanup_memory:
	free(packet);
return_ret:
	return ret;
}


int CPUGame(SOCKET sock, char* player_move_s, char* cpu_move_s, int *winning_player)
{
	int player_move = 0, cpu_move = 0;
	int err = 0, ret = 0;

	time_t t;

	err = GetMoveFromClient(sock, player_move_s, &player_move);
	if (err != 0) { ret = err;  goto return_ret; }

	/*------------------------------- Play CPU match ---------------------------------*/
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	cpu_move = rand() % 5 + 1;
	switch (cpu_move)
	{
	case ROCK:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "ROCK");
		break;
	case PAPER:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "PAPER");
		break;
	case SCISSORS:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "SCISSORS");
		break;
	case LIZARD:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "LIZARD");
		break;
	case SPOCK:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "SPOCK");
		break;
	}
	if (err != 0)
	{
		printf("Error: strcpy failed\n");
		ret = ERROR_CODE;
		goto return_ret;
	}

	*winning_player = PlayMatch(player_move, cpu_move);

return_ret:
	return ret;
}


int VersusGame(SOCKET sock, char* player_move_s, char* opp_move_s, int *winning_player)
{
	
	//variables
	char message_type[MAX_MESSAGE];
	char message_send[MAX_MESSAGE];
	
	int retVal = 0, err = 0, exit_code =0, flag = 0;

	int player_move, wait_code;
	int opponent_mov = -1;
	char opponent_move_s[10] = "";

	sendthread_s packet;

	// find opponent barrier
	retVal = findOpponentBarrier();

	if (retVal = ERROR_CODE) return ERROR_CODE;
	// Opponent wasn't found
	else if (retVal == OPPONENT_WASENT_FOUND)
	{
		err = sprintf_s(message_send, MAX_MESSAGE, "%s:\n", SERVER_NO_OPPONENTS);
		if (err == 0 || err == EOF)
		{
			printf("Error: can't create the message for the client\n");
			return ERROR_CODE;
		}
		packet.array_t = message_send;
		packet.array_size = strlen(message_send);

		//activate the send thread and get his exit code
		exit_code = ActivateThread((void*)&packet, 1, SENDRECV_WAITTIME);
		//if the thread setup failed or the thread function itself failed
		if (exit_code != 0)  return exit_code;
		return 0;
	}
	else
	{
		// Opponent found
		flag = MakeSureFileExist();
		if (flag == ERROR_CODE) return ERROR_CODE;

		if (flag == 1) // the file was already open
		{
			/* Ask client for his move */
			err = GetMoveFromClient(sock, player_move_s, &player_move);
			if (err != 0) return err;
			
			/* Write move to GameSession.txt */
			retVal = ReadOrWriteToGameSassionFile(player_move_s, &player_move, WRITE_TO_GAMESESSION);

			/* Signal opponent thread */
			retVal = ReleaseSemaphore(com_sem[!flag], 1, NULL);
			if (FALSE == retVal)
			{
				printf("Error when releasing find_opp_sem\n");

				return ERROR_CODE;
			}

			/* Wait opponent thread */
			wait_code = WaitForSingleObject(com_sem[flag], INFINITE);
			if (WAIT_OBJECT_0 != wait_code)
			{
				printf("Error when waiting for com_sem[0]\n");
				return ERROR_CODE;
			}

			/* Read Opponent Move */
			retVal = ReadOrWriteToGameSassionFile(opponent_move_s, &opponent_mov, READ_FROM_GAMESESSION);
			
			/* Signal opponent thread */
			retVal = ReleaseSemaphore(com_sem[!flag], 1, NULL);
			if (FALSE == retVal)
			{
				printf("Error when releasing find_opp_sem\n");

				return ERROR_CODE;
			}

		}
		else // i opened the file
		{

			/* Ask client for his move */
			err = GetMoveFromClient(sock, player_move_s, &player_move);
			if (err != 0) return err;

			/* Wait opponent thread */
			wait_code = WaitForSingleObject(com_sem[flag], INFINITE);
			if (WAIT_OBJECT_0 != wait_code)
			{
				printf("Error when waiting for com_sem[0]\n");
				return ERROR_CODE;
			}

			/* Read Opponent Move */
			retVal = ReadOrWriteToGameSassionFile(opponent_move_s, &opponent_mov, READ_FROM_GAMESESSION);

			/* Write move to GameSession.txt */
			retVal = ReadOrWriteToGameSassionFile(player_move_s, &player_move, WRITE_TO_GAMESESSION);

			/* Signal opponent thread */
			retVal = ReleaseSemaphore(com_sem[!flag], 1, NULL);
			if (FALSE == retVal)
			{
				printf("Error when releasing find_opp_sem\n");

				return ERROR_CODE;
			}

			/* Wait opponent thread */
			wait_code = WaitForSingleObject(com_sem[flag], INFINITE);
			if (WAIT_OBJECT_0 != wait_code)
			{
				printf("Error when waiting for com_sem[0]\n");
				return ERROR_CODE;
			}

			/* Remove file */
			if (remove("HELLO.txt") == -1)
			{
				printf("Error in deleting GameSession.txt file");
				return ERROR_CODE;
			}
			
		}
		/* Calculate result */
		*winning_player = PlayMatch(player_move, opponent_mov);

		return 0;
	}
}

int findOpponentBarrier()
{
	DWORD wait_code;
	BOOL ret_val;
	int ret = 0;

	wait_code = WaitForSingleObject(find_opp_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for find_opp_mutex\n");
		return ERROR_CODE;
	}
	
	/* critical section */
	barrier_count = barrier_count + 1;

	if (barrier_count == MAX_NUM_CLIENTS)
	{
		// if this is the last thread to reach the barrier release all.
		ret_val = ReleaseSemaphore(find_opp_sem, MAX_NUM_CLIENTS, NULL);
		if (FALSE == ret_val)
		{
			printf("Error when releasing find_opp_sem\n");
			
			ret = ERROR_CODE;
		}
	}

	ret_val = ReleaseMutex(find_opp_mutex);
	if (FALSE == ret_val)
	{
		printf("Error when releasing find_opp_mutex\n");
		return ERROR_CODE;
	}

	// if the up barrier_sem failed, return ERROR_CODE
	if (ret == ERROR_CODE)
		return ERROR_CODE;

	wait_code = WaitForSingleObject(find_opp_sem, WAIT_FOR_OPP_TIME); // Need to change the timeout to 30 sec

	if (WAIT_OBJECT_0 == wait_code)		return OPPONENT_FOUND;
	else if (WAIT_TIMEOUT == wait_code)
	{
		// reset the count if no opponent found
		wait_code = WaitForSingleObject(find_opp_mutex, INFINITE);
		if (WAIT_OBJECT_0 != wait_code)
		{
			printf("Error when waiting for find_opp_mutex\n");
			return ERROR_CODE;
		}

		/* critical section */
		barrier_count = 0;
		if(barrier_count == 1) ret = OPPONENT_WASENT_FOUND;
		else ret = OPPONENT_FOUND;

		ret_val = ReleaseMutex(find_opp_mutex);
		if (FALSE == ret_val)
		{
			printf("Error when releasing find_opp_mutex\n");
			return ERROR_CODE;
		}

		return ret;
	}
	else
	{
		printf("Error when waiting for find_opp_sem\n");
		return ERROR_CODE;
	}

}

int MakeSureFileExist()
{
	FILE* fp = NULL;
	int wait_code =0, flag =0, retVal;
	char fname[] = "GameSession.txt";

	wait_code = WaitForSingleObject(com_file_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for com_file_mutex\n");
		return ERROR_CODE;
	}

	/* critical section */
	if (FileExists(fname)) {
		// file exists
		flag = 1;
	}
	else {
		// file doesn't exist
		retVal = fopen_s(&fp,fname,"w");
		if (retVal != 0)
		{
			printf("Error while trying to create %s\n",fname);
			flag = ERROR_CODE;
		}
		fclose(fp);
		flag = 0;
	}

	retVal = ReleaseMutex(com_file_mutex);
	if (FALSE == retVal)
	{
		printf("Error when releasing com_file_mutex\n");
		return ERROR_CODE;
	}
	return flag;

}

bool FileExists(const TCHAR *fileName)
{
	DWORD       fileAttr;

	fileAttr = GetFileAttributes(fileName);
	if (0xFFFFFFFF == fileAttr)
		return false;
	return true;
}


int ReadOrWriteToGameSassionFile(char* player_move_s, int* player_move_i, int read_write)
{
	char fname[] = "GameSession.txt";
	FILE* fp;
	int wait_code,retVal =0, ret =0;

	wait_code = WaitForSingleObject(com_file_mutex, INFINITE);
	if (WAIT_OBJECT_0 != wait_code)
	{
		printf("Error when waiting for com_file_mutex\n");
		return ERROR_CODE;
	}

	/* critical section */

	if (read_write == WRITE_TO_GAMESESSION)
	{
		/* Write */

			retVal = fopen_s(&fp, fname, "w");
			if (retVal != 0)
			{
				printf("Error while trying to open %s\n", fname);
				ret = ERROR_CODE;
			}
			fprintf(fp, "%s,%d", player_move_s, *player_move_i);
			fclose(fp);
	}
	else
	{
		/* Read */
		retVal = fopen_s(&fp, fname, "r");
		if (retVal != 0)
		{
			printf("Error while trying to open %s\n", fname);
			ret = ERROR_CODE;
		}
		retVal = fsacnf_s(fp, "%s,%d", player_move_s,8, player_move_i);
		fclose(fp);
	
	}
	retVal = ReleaseMutex(com_file_mutex);
	if (FALSE == retVal)
	{
		printf("Error when releasing com_file_mutex\n");
		return ERROR_CODE;
	}

	return ret;

}

int EndGameStatus(	SOCKET sock, char *username, char *other_player, char *my_move,
					char *other_move, int winning_player, int *replay)
{
	char message_send[MAX_MESSAGE];
	char message_type[MAX_MESSAGE];
	char message_info[MAX_MESSAGE];

	sendthread_s *packet;

	int err = 0, ret = 0, exit_code = 0;

	//malloc for the sendthread_s struct
	packet = (sendthread_s*)malloc(sizeof(sendthread_s));
	if (packet == NULL)
	{
		printf("ERROR: allocate memory for thread arg\n");
		ret = ERROR_CODE;
		goto return_ret;
	}

	//intialize the socket to the thread functions
	packet->sock = sock;

	/*------------------------------- send SERVER_GAME_RESULT ---------------------------------*/
	//if the client won
	if (winning_player == 1)
		err = sprintf_s(message_send, MAX_MESSAGE, "%s:%s;%s;%s;%s\n", SERVER_GAME_RESULTS,
			other_player, other_move, my_move, username);
	//if the cpu won
	else if (winning_player == 2)
		err = sprintf_s(message_send, MAX_MESSAGE, "%s:%s;%s;%s;%s\n", SERVER_GAME_RESULTS,
			other_player, other_move, my_move, other_player);
	//else its a tie
	else
		err = sprintf_s(message_send, MAX_MESSAGE, "%s:%s;%s;%s;%s\n", SERVER_GAME_RESULTS,
			other_player, other_move, my_move, "No one");

	if (err == 0 || err == EOF)
	{
		printf("Error: can't create the message for the client\n");
		ret = ERROR_CODE;
		goto cleanup_memory;
	}

	packet->array_t = message_send;
	packet->array_size = strlen(message_send);

	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto cleanup_memory; }

	/*------------------------------- send SERVER_GAME_OVER_MENU ---------------------------------*/
	//create the server game over menu message to the client
	err = sprintf_s(message_send, MAX_MESSAGE, "%s:\n", SERVER_GAME_OVER_MENU);
	if (err == 0 || err == EOF)
	{
		printf("Error: can't create the message for the client\n");
		ret = ERROR_CODE;
		goto cleanup_memory;
	}

	packet->array_t = message_send;
	packet->array_size = strlen(message_send);

	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto cleanup_memory; }

	/*------------------------------- recive client selection for game over menu ---------------------------------*/
	packet->array_t = NULL;
	packet->array_size = 0;

	//activate the recv thread and get his exit code
	exit_code = ActivateThread((void*)packet, 0, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto cleanup_memory; }

	//get the message type and the information
	err = MessageCut(packet->array_t, packet->array_size, message_type, message_info);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto main_cleanup1; }

	//check for the message type to update the menu selection
	if (STRINGS_ARE_EQUAL(message_type, CLIENT_REPLAY))
		*replay = 1;
	else if (STRINGS_ARE_EQUAL(message_type, CLIENT_MAIN_MENU))
		*replay = 0;
	else
	{
		printf("Error: message recieved from the client doesnt match with the protocol\n");
		ret = ERROR_CODE;
		goto main_cleanup1;
	}



main_cleanup1:
	free(packet->array_t);
cleanup_memory:
	free(packet);
return_ret:
	return ret;
}


int GetMoveFromClient(SOCKET sock, char* player_move_s, int *player_move)
{
	//variables
	char message_type[MAX_MESSAGE];
	char message_send[MAX_MESSAGE];

	sendthread_s *packet;

	int size_arr = 0, ret = 0, exit_code = 0, err = 0;


	//malloc for the sendthread_s struct
	packet = (sendthread_s*)malloc(sizeof(sendthread_s));
	if (packet == NULL)
	{
		printf("ERROR: allocate memory for thread arg\n");
		ret = ERROR_CODE;
		goto return_ret;
	}

	//intialize the socket to the thread functions
	packet->sock = sock;

	/*----------------------------send SERVER_PLAYER_MOVE_REQUEST-----------------------------*/
	err = sprintf_s(message_send, MAX_MESSAGE, "%s:\n", SERVER_PLAYER_MOVE_REQUEST);
	if (err == 0 || err == EOF)
	{
		printf("Error: can't create the message for the client\n");
		ret = ERROR_CODE;
		goto cleanup_memory;
	}

	packet->array_t = message_send;
	packet->array_size = strlen(message_send);

	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto cleanup_memory; }


	/*-------------------------------recv CLIENT_PLAYER_MOVE---------------------------------*/
	packet->array_t = NULL;
	packet->array_size = 0;

	//activate the recv thread and get his exit code
	exit_code = ActivateThread((void*)packet, 0, USER_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto cleanup_memory; }

	//get the message type and the information
	err = MessageCut(packet->array_t, packet->array_size, message_type, player_move_s);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto cleanup_array; }

	if (!STRINGS_ARE_EQUAL(message_type, CLIENT_PLAYER_MOVE))
	{
		printf("Error: message recieved from the client doesnt match with the protocol\n");
		ret = ERROR_CODE;
		goto cleanup_array;
	}

	if (STRINGS_ARE_EQUAL(player_move_s, "ROCK"))
		*player_move = ROCK;
	else if (STRINGS_ARE_EQUAL(player_move_s, "PAPER"))
		*player_move = PAPER;
	else if (STRINGS_ARE_EQUAL(player_move_s, "SCISSORS"))
		*player_move = SCISSORS;
	else if (STRINGS_ARE_EQUAL(player_move_s, "LIZARD"))
		*player_move = LIZARD;
	else if (STRINGS_ARE_EQUAL(player_move_s, "SPOCK"))
		*player_move = SPOCK;
	else
	{
		printf("Error: client move doesnt match with the protocol\n");
		ret = ERROR_CODE;
		goto cleanup_array;
	}

cleanup_array:
	free(packet->array_t);
cleanup_memory:
	free(packet);
return_ret:
	return ret;
}


int PlayMatch(int player1_move, int player2_move)
{
	//if the first player is a rock
	if (player1_move == ROCK)
	{
		//beats scissors and lizard
		if (player2_move == SCISSORS || player2_move == LIZARD)
			return 1;
		//beaten by paper and spock
		else if (player2_move == SPOCK || player2_move == PAPER)
			return 2;
		//a tie
		else
			return 0;
	}
	//if the first player is a paper
	else if (player1_move == PAPER)
	{
		//beats rock and spock
		if (player2_move == ROCK || player2_move == SPOCK)
			return 1;
		//beaten by lizard and scissors
		else if (player2_move == LIZARD || player2_move == SCISSORS)
			return 2;
		//a tie
		else
			return 0;
	}
	//if the first player is scissors
	else if (player1_move == SCISSORS)
	{
		//beats paper and lizard
		if (player2_move == PAPER || player2_move == LIZARD)
			return 1;
		//beaten by rock and spock
		else if (player2_move == ROCK || player2_move == SPOCK)
			return 2;
		//a tie
		else
			return 0;
	}
	//if the first player is a lizard
	else if (player1_move == LIZARD)
	{
		//beats paper and spock
		if (player2_move == PAPER || player2_move == SPOCK)
			return 1;
		//beaten by rock and scissors
		else if (player2_move == ROCK || player2_move == SCISSORS)
			return 2;
		//a tie
		else
			return 0;
	}
	//if the first player is a spock
	else
	{
		//beats scissors and rock
		if (player2_move == SCISSORS || player2_move == ROCK)
			return 1;
		//beaten by paper and lizard
		else if (player2_move == LIZARD || player2_move == PAPER)
			return 2;
		//a tie
		else
			return 0;
	}
}

