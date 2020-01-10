
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
		player_move = 1;
	else if (STRINGS_ARE_EQUAL(message_info, "PAPER"))
		player_move = 2;
	else if (STRINGS_ARE_EQUAL(message_info, "SCISSORS"))
		player_move = 3;
	else if (STRINGS_ARE_EQUAL(message_info, "LIZARD"))
		player_move = 4;
	else if (STRINGS_ARE_EQUAL(message_info, "SPOCK"))
		player_move = 5;
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


int ActivateThread(void *arg, int recv_or_send, int waittime)
{
	HANDLE thread_handle = NULL;
	LPWORD thread_id;

	int ret = 0, wait_ret = 0, err_flag = 0;
	DWORD exit_code;

	if (recv_or_send == 0)
		//call the recevie thread
		thread_handle = CreateThreadSimple((LPTHREAD_START_ROUTINE)RecvThread, &thread_id, arg);
	else
		//call the send thread
		thread_handle = CreateThreadSimple((LPTHREAD_START_ROUTINE)SendThread, &thread_id, arg);

	//create thread simple failed
	if (thread_handle == NULL)
	{
		printf("Error creating RecvThread\n");
		ret = ERROR_CODE;
		goto return_goto;
	}

	//wait for the thread with timeout of 15 seconds
	wait_ret = WaitForSingleObject(thread_handle, waittime);

	//if there is a timeout, terminate the thread and close the thread
	if (wait_ret == WAIT_TIMEOUT)
	{
		printf("Error: wait timeout to send the message\n");
		printf("Terminating thread and exiting\n");
		err_flag = TerminateThread(thread_handle, exit_code);
		ret = ERROR_CODE;
		goto main_cleanup1;
	}
	//the wait for signle object failed, close the thread
	else if (wait_ret != WAIT_OBJECT_0)
	{
		printf("Error while waiting using WaitForSingleObject\n");
		ret = ERROR_CODE;
		goto main_cleanup1;
	}

	//if the thread is runnig correctly, get the exit code
	err_flag = GetExitCodeThread(thread_handle, exit_code);
	if (err_flag == 0)
	{
		printf("Error while getting exit code from thread\n");
		ret = ERROR_CODE;
		goto main_cleanup1;
	}

	ret = exit_code;


main_cleanup1:
	CloseHandle(thread_handle);
return_goto:
	return ret;
}