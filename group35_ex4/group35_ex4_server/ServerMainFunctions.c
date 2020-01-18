
#include "ServerMainFunctions.h"


int serverMain()
{

	int Ind;
	int retVal;
	int ret = 0 ;
	SOCKET acceptSocket = INVALID_SOCKET;

	LPWORD accept_thread_id, exit_thread_id;
	HANDLE accept_exit_ThreadHandle[2] = { NULL };
	AcceptSocketParams acceptParam;


	char recvMessage[MAX_MESSAGE] = "";
	char messageType[MAX_MESSAGE] = "";
	char messageInfo[MAX_MESSAGE] = "";

	sendthread_s recive;

	// Initialize Winsock.
	WSADATA wsaData;
	int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (StartupRes != NO_ERROR)
	{
		printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());
		// Tell the user that we could not find a usable WinSock DLL.                                  
		return ERROR_CODE;
	}
	/* The WinSock DLL is acceptable. Proceed. */

	// Initialize the listening socket 
	retVal = initializeListeningSocket(&acceptParam);
	if (retVal == ERROR_CODE)
	{
		ret = ERROR_CODE;
		goto server_cleanup_1;
	}
	
	// Initialize all mutex and semaphore 
	retVal = initializeSemaphores();
	if (retVal == ERROR_CODE)
	{
		ret = ERROR_CODE;
		goto server_cleanup_2;
	}
	
	//Intialize the leaderboard struct
	retVal = IntializeLeaderboard(&first_player);
	if (retVal == ERROR_CODE)
	{
		ret = ERROR_CODE;
		goto server_cleanup_2;
	}

	accept_exit_ThreadHandle[1] = CreateThreadSimple((LPTHREAD_START_ROUTINE)CheckExitThread, &exit_thread_id, NULL);
	if (accept_exit_ThreadHandle[1] == NULL)
	{
		printf("Error creating CheckExitThread\n");
		goto server_cleanup_3;
	}


	while (TRUE)
	{
	
		//start the accept thread 
		accept_exit_ThreadHandle[0] = CreateThreadSimple((LPTHREAD_START_ROUTINE)AcceptClientThread, &accept_thread_id, &acceptParam);
		if (accept_exit_ThreadHandle[2] == NULL)
		{
			printf("Error creating CheckExitThread\n");
			goto server_cleanup_4;
		}
		// wait for ether exit or client trying to connect
		int retVal = WaitForMultipleObjects(2, accept_exit_ThreadHandle, false, INFINITE);
		if ((retVal != WAIT_OBJECT_0) && (retVal != WAIT_OBJECT_0 + 1))
		{
			printf("Error while waiting using WaitForMultipleObjects\n");
			ret = ERROR_CODE;
			goto server_cleanup_5;
		}
		// check Which thread has finished
		if (retVal - WAIT_OBJECT_0 == 1)
		{
			break;
		}


		Ind = FindFirstUnusedThreadSlot();
		if (Ind == MAX_NUM_CLIENTS) //no slot is available
		{
			printf("No slots available for client, dropping the connection.\n");
			closesocket(acceptSocket); //Closing the socket, dropping the connection.
		}
	
		else
		{
			(ThreadInputs[Ind]).client_socket = acceptParam.AcceptSocket;
			ThreadInputs[Ind].index = Ind;
			//ThreadHandles[Ind] = CreateThreadSimple((LPTHREAD_START_ROUTINE)ServiceThread, &(ThreadInputs[Ind]), NULL);
			ServiceThread((void*)&(ThreadInputs[Ind]));
			if (accept_exit_ThreadHandle[1] == NULL)
			{
				printf("Error creating CheckExitThread\n");
				goto server_cleanup_3;
			}

		}
	
	}

	/* EXIT procedure */



server_cleanup_5:
	// close acceptThreadHandle	
	CloseHandle(accept_exit_ThreadHandle[0]);
server_cleanup_4 :
	// close exitThreadHandle	
	CloseHandle(accept_exit_ThreadHandle[1]);
server_cleanup_3:
	closeSemaphores();
server_cleanup_2:
	if(closesocket(acceptParam.mainSocket) == SOCKET_ERROR)
		printf("Failed to close MainSocket, error %ld. Ending program\n", WSAGetLastError());

server_cleanup_1: //Close WINSOCK 
	if (WSACleanup() == SOCKET_ERROR)
		printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
	return ret; 
}


int initializeListeningSocket(AcceptSocketParams *acceptParams)
{
	int bindRes, ListenRes;
	unsigned long Address;
	int ret = 0;


	SOCKADDR_IN service;

	SOCKET *MainSocket = &acceptParams->mainSocket;

	*MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (*MainSocket == INVALID_SOCKET)
	{
		printf("Error at socket( ): %ld\n", WSAGetLastError());
		return ERROR_CODE;
	}

	// create the int of the ip addres
	Address = inet_addr(SERVER_ADDRESS_STR);
	if (Address == INADDR_NONE)
	{
		printf("The string \"%s\" cannot be converted into an ip address. ending program.\n",
			SERVER_ADDRESS_STR);
		ret = ERROR_CODE;
		goto cleanup_1;
	}

	// set the sockadder struck fields
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = Address;
	service.sin_port = htons(SERVER_PORT); //The htons function converts a u_short from host to TCP/IP network byte order 
									   //( which is big-endian ).
	/*
		The three lines following the declaration of sockaddr_in service are used to set up
		the sockaddr structure:
		AF_INET is the Internet address family.
		"127.0.0.1" is the local IP address to which the socket will be bound.
		2345 is the port number to which the socket will be bound.
	*/

	// Call the bind function, passing the created socket and the sockaddr_in structure as parameters. 
	// Check for general errors.
	bindRes = bind(*MainSocket, (SOCKADDR*)&service, sizeof(service));
	if (bindRes == SOCKET_ERROR)
	{
		printf("bind( ) failed with error %ld. Ending program\n", WSAGetLastError());
		ret = ERROR_CODE;
		goto cleanup_1;
	}


	// Listen on the Socket.
	ListenRes = listen(*MainSocket, SOMAXCONN);
	if (ListenRes == SOCKET_ERROR)
	{
		printf("Failed listening on socket, error %ld.\n", WSAGetLastError());
		ret = ERROR_CODE;
		goto cleanup_1;
	}

	return ret;

cleanup_1:
	if (closesocket(MainSocket) == SOCKET_ERROR)
		printf("Failed to close MainSocket, error %ld. Ending program\n", WSAGetLastError());

	return ret;
}


int initializeSemaphores()
{


	// Initialize Find opponent barrier
	find_opp_event = CreateEvent(NULL ,TRUE,FALSE, NULL);
	if (find_opp_event == NULL)
	{
		printf("Error creating find_opp_event\n");
		goto cleanup_1;
	}

	find_opp_mutex = CreateSemaphore(NULL, 1, 1, NULL);
	if (find_opp_mutex == NULL)
	{
		printf("Error creating find_opp_mutex\n");
		goto cleanup_2;
	}

	com_file_mutex = CreateMutex(NULL, FALSE, NULL);
	if (com_file_mutex == NULL)
	{
		printf("Error creating com_file_mutex\n");
		goto cleanup_3;
	}

	com_sem[0] = CreateSemaphore(NULL, 0, 1, NULL);
	if (com_sem[0] == NULL)
	{
		printf("Error creating com_sem[0]\n");
		goto cleanup_4;
	}
	com_sem[1] = CreateSemaphore(NULL, 0, 1, NULL);
	if (com_sem[1] == NULL)
	{
		printf("Error creating com_sem[1]\n");
		goto cleanup_5;
	}
	// all inits went well
	return 0;

cleanup_5:
	CloseHandle(com_sem[0]);
cleanup_4:
	CloseHandle(com_file_mutex);
cleanup_3:
	CloseHandle(find_opp_mutex);
cleanup_2:
	CloseHandle(find_opp_event);
cleanup_1:
	return ERROR_CODE;
}


static int FindFirstUnusedThreadSlot()
{
	int Ind;

	for (Ind = 0; Ind < MAX_NUM_CLIENTS; Ind++)
	{
		if (ThreadHandles[Ind] == NULL)
			break;
		else
		{
			// poll to check if thread finished running:
			DWORD Res = WaitForSingleObject(ThreadHandles[Ind], 0);

			if (Res == WAIT_OBJECT_0) // this thread finished running
			{
				CloseHandle(ThreadHandles[Ind]);
				ThreadHandles[Ind] = NULL;
				break;
			}
		}
	}

	return Ind;
}


void closeSemaphores()
{

	CloseHandle(com_sem[1]);
	CloseHandle(com_sem[0]);
	CloseHandle(com_file_mutex);
	CloseHandle(find_opp_mutex);
	CloseHandle(find_opp_event);
}


int IntializeLeaderboard(leaderboard_player **first_p)
{
	FILE *fp;

	char first_line[MAX_LINE];
	char player_line[MAX_LINE];

	int ret = 0, err = 0;
	err = fopen_s(&fp, "Leaderboard.csv", "r");

	leaderboard_player *current_player = NULL;

	//the file exists
	if (fp)
	{
		//ignoring the first line
		if (fgets(first_line, 26, fp) == NULL)
		{
			printf("Error: reading from file\n");
			ret = ERROR_CODE;
			goto close_file;
		}
		//go over the file until we get to the EOF
		while (fgets(player_line, MAX_LINE, fp) != NULL)
		{
			//new item on the list
			if (*first_p == NULL)
			{
				(*first_p) = (leaderboard_player*)malloc(sizeof(leaderboard_player));
				if ((*first_p) == NULL)
				{
					printf("Error: allocating memory for the leaderboard struct\n");
					ret = ERROR_CODE;
					goto close_file;
				}
				current_player = *first_p;
			}
			//not a new item
			else
			{
				current_player->next = (leaderboard_player*)malloc(sizeof(leaderboard_player));
				if (current_player->next == NULL)
				{
					printf("Error: allocating memory for the leaderboard struct\n");
					ret = ERROR_CODE;
					goto free_struct;
				}
				current_player = current_player->next;
			}


			//now the player is in the last place on the list so far (he has the worst ratio)
			//this information is for the last player
			ReplaceCommaStr(player_line);

			err = sscanf_s(player_line, "%s%d%d%lf\n", current_player->username, MAX_USERNAME + 1,
				&(current_player->won), &(current_player->lost), &(current_player->ratio));
			if (err == EOF || err == 0)
			{
				printf("Error: getting the information of the leaderboard using sscanf\n");
				ret = ERROR_CODE;
				goto free_struct;
			}
			//if there is no ratio
			else if (err == 3)
			{
				if (current_player->won > 0)
					current_player->ratio = -1;
				else
					current_player->ratio = 0;
			}
			current_player->next = NULL;
		}
	}
	//the file doesnt exist
	else
	{
		err = fopen_s(&fp, "Leaderboard.csv", "w");
		if (fp == NULL)
		{
			printf("Error: creating the leaderboard file\n");
			ret = ERROR_CODE;
			goto ret_goto;
		}
		err = fputs("Name,Won,Lost,W/L Ratio\n", fp);
		if (err == EOF)
		{
			printf("Error: writing to the leaderboard file\n");
			ret = ERROR_CODE;
			goto ret_goto;
		}
	}
	goto close_file;

free_struct:
	err = FreeLeaderboard(first_p);
close_file:
	fclose(fp);
ret_goto:
	return ret;
}


int FreeLeaderboard(leaderboard_player **first_p)
{
	leaderboard_player *cur = NULL;

	while (*first_p != NULL)
	{
		cur = *first_p;
		*first_p = (*first_p)->next;
		free(cur);
	}
	return 0;
}


void ReplaceCommaStr(char *line_str)
{
	int i = 0;
	for (i = 0; line_str[i] != '\0'; i++)
	{
		if (line_str[i] == ',')
		{
			line_str[i] = ' ';
		}
	}
}