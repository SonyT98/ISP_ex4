
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
	err = sscanf_s(message_send, "%s:\n", SERVER_APPROVED, sizeof(SERVER_APPROVED));
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
	err = sscanf_s(message_send, "%s:\n", SERVER_MAIN_MENU, sizeof(SERVER_MAIN_MENU));
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


int CPUGame(SOCKET sock, int *replay)
{
	//variables
	char message_type[MAX_MESSAGE];
	char message_info[MAX_MESSAGE];
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
	err = sscanf_s(message_send, "%s:\n", SERVER_PLAYER_MOVE_REQUEST, sizeof(SERVER_PLAYER_MOVE_REQUEST));
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
	err = MessageCut(packet->array_t, packet->array_size, message_type, message_info);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto cleanup_memory; }

	if (!STRINGS_ARE_EQUAL(message_type, CLIENT_PLAYER_MOVE))
	{
		printf("Error: message recieved from the client doesnt match with the protocol\n");
		ret = ERROR_CODE;
		goto cleanup_memory;
	}

	if (STRINGS_ARE_EQUAL(message_info, "ROCK"))
		player_move = ROCK;
	else if (STRINGS_ARE_EQUAL(message_info, "PAPER"))
		player_move = PAPER;
	else if (STRINGS_ARE_EQUAL(message_info, "SCISSORS"))
		player_move = SCISSORS;
	else if (STRINGS_ARE_EQUAL(message_info, "LIZARD"))
		player_move = LIZARD;
	else if (STRINGS_ARE_EQUAL(message_info, "SPOCK"))
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



cleanup_memory:
	free(packet);
return_ret:
	return ret;
}


int MessageCut(char *message, int message_size, char* message_type, char *info)
{
	int i = 0;
	int info_or_type = 0;
	int type_size = 0;

	for (i = 0; i < message_size; i++)
	{
		//if this is a message type
		if (info_or_type == 0)
		{
			message_type[i] = message[i];
			type_size = i+1;
			if (message_type[i] == ':')
			{
				message_type[i] = '\0';
				info_or_type = 1;
			}
		}
		//this is a info type
		else
			info[i - type_size] = message[i];
	}
	//if we didnt got to ':' char this isnt a correct message
	if (info_or_type == 0)
	{
		printf("Error: The message received isn't correct\n");
		return ERROR_CODE;
	}
	// set info as string
	else
		info[i - type_size-1] = '\0';
	return 0;
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