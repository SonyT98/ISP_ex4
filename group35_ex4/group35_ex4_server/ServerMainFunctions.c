
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "ServerMainFunctions.h"

HANDLE ThreadHandles[MAX_NUM_CLINTS];
ServiceThreadParams ThreadInputs[MAX_NUM_CLINTS];

int serverMain()
{

	int Ind;
	int Loop;
	SOCKET MainSocket = INVALID_SOCKET;
	unsigned long Address;
	SOCKADDR_IN service;
	int bindRes;
	int ListenRes;
	int retVal;
	int ret = 0 ;
	SOCKET acceptSocket = INVALID_SOCKET;

	LPWORD accept_thread_id, exit_thread_id;
	HANDLE accept_exit_ThreadHandle[2] = { NULL };
	AcceptSocketParams acceptParam;

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

	
	retVal = initializeListeningSocket(&acceptParam);
	if (retVal = ERROR_CODE)
	{
		ret = ERROR_CODE;
		goto server_cleanup_1;
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
		if (Ind == MAX_NUM_CLINTS) //no slot is available
		{
			printf("No slots available for client, dropping the connection.\n");
			closesocket(acceptSocket); //Closing the socket, dropping the connection.
		}
	
		else
		{
			(ThreadInputs[Ind]).client_socket = acceptSocket;
			ThreadInputs[Ind].index = Ind;
			ThreadHandles[Ind] = CreateThreadSimple((LPTHREAD_START_ROUTINE)ServiceThread, &(ThreadInputs[Ind]), NULL);
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
	//Close all semaphore and Handles!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
server_cleanup_2:
	if(closesocket(MainSocket) == SOCKET_ERROR)
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


static int FindFirstUnusedThreadSlot()
{
	int Ind;

	for (Ind = 0; Ind < MAX_NUM_CLINTS; Ind++)
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