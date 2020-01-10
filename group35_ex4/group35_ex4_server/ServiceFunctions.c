
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
	//variables
	char message_type[MAX_MESSAGE];
	char message_send[MAX_MESSAGE];

	sendthread_s *packet;

	int size_arr = 0, ret = 0, exit_code = 0, err = 0;
	int player_move = 0, cpu_move = 0;

	time_t t;


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
	err = sprintf_s(message_send,MAX_MESSAGE, "%s:\n", SERVER_PLAYER_MOVE_REQUEST);
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
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto cleanup_memory; }

	if (!STRINGS_ARE_EQUAL(message_type, CLIENT_PLAYER_MOVE))
	{
		printf("Error: message recieved from the client doesnt match with the protocol\n");
		ret = ERROR_CODE;
		goto cleanup_memory;
	}

	if (STRINGS_ARE_EQUAL(player_move_s, "ROCK"))
		player_move = ROCK;
	else if (STRINGS_ARE_EQUAL(player_move_s, "PAPER"))
		player_move = PAPER;
	else if (STRINGS_ARE_EQUAL(player_move_s, "SCISSORS"))
		player_move = SCISSORS;
	else if (STRINGS_ARE_EQUAL(player_move_s, "LIZARD"))
		player_move = LIZARD;
	else if (STRINGS_ARE_EQUAL(player_move_s, "SPOCK"))
		player_move = SPOCK;
	else
	{
		printf("Error: client move doesnt match with the protocol\n");
		ret = ERROR_CODE;
		goto cleanup_memory;
	}

	free(packet->array_t);

	/*------------------------------- Play CPU match ---------------------------------*/
	/* Intializes random number generator */
	srand((unsigned)time(&t));

	cpu_move = rand() % 5 + 1;
	switch (cpu_move)
	{
	case ROCK:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "ROCK");
	case PAPER:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "PAPER");
	case SCISSORS:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "SCISSORS");
	case LIZARD:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "LIZARD");
	case SPOCK:
		err = strcpy_s(cpu_move_s, MAX_MESSAGE, "SPOCK");
	}
	if (err == 0)
	{
		printf("Error: strcpy failed\n");
		ret = ERROR_CODE;
		goto cleanup_memory;
	}

	*winning_player = PlayMatch(player_move, cpu_move);

cleanup_memory:
	free(packet);
return_ret:
	return ret;
}

int EndGameStatus(SOCKET sock, char *username, char *other_player, char *my_move,
	char *other_move, int winning_player, int *replay)
{
	/*------------------------------- send SERVER_GAME_RESULT ---------------------------------*/
	//if the client won
	if (winning_player == 1)
	{
		err = sscanf_s(message_send, "%s:%s;%s;%s;%s\n", SERVER_GAME_RESULTS, sizeof(SERVER_GAME_RESULTS),
			"CPU", sizeof("CPU"), cpu_move_string, strlen(cpu_move_string) + 1, message_info, strlen(message_info) + 1,
			username, strlen(username) + 1);
		if (err == 0 || err == EOF)
		{
			printf("Error: can't create the message for the client\n");
			ret = ERROR_CODE;
			goto cleanup_memory;
		}
	}
	//if the cpu won
	else if (winning_player == 2)
	{
		err = sscanf_s(message_send, "%s:%s;%s;%s;%s\n", SERVER_GAME_RESULTS, sizeof(SERVER_GAME_RESULTS),
			"CPU", sizeof("CPU"), cpu_move_string, strlen(cpu_move_string) + 1, message_info, strlen(message_info) + 1,
			"CPU", sizeof("CPU"));
		if (err == 0 || err == EOF)
		{
			printf("Error: can't create the message for the client\n");
			ret = ERROR_CODE;
			goto cleanup_memory;
		}
	}
	else
	{
		err = sscanf_s(message_send, "%s:%s;%s;%s;%s\n", SERVER_GAME_RESULTS, sizeof(SERVER_GAME_RESULTS),
			"CPU", sizeof("CPU"), cpu_move_string, strlen(cpu_move_string) + 1, message_info, strlen(message_info) + 1,
			"No One", sizeof("No one"));
		if (err == 0 || err == EOF)
		{
			printf("Error: can't create the message for the client\n");
			ret = ERROR_CODE;
			goto cleanup_memory;
		}
	}

	packet->array_t = message_send;
	packet->array_size = strlen(message_send);

	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code != 0) { ret = exit_code;  goto cleanup_memory; }
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