
#include "ServerMainFunctions.h"


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


	// Create a socket for TCP/IP.    
	MainSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (MainSocket == INVALID_SOCKET)
	{
		printf("Error at socket( ): %ld\n", WSAGetLastError());
		retVal = ERROR_CODE;
		goto server_cleanup_1;
	}


	// Listen on the Socket.
	ListenRes = listen(MainSocket, SOMAXCONN);
	if (ListenRes == SOCKET_ERROR)
	{
		printf("Failed listening on socket, error %ld.\n", WSAGetLastError());
		retVal = ERROR_CODE;
		goto server_cleanup_2;
	}
	// Initialize all semaphore and mutexes.
	if (intializeServerResources() == ERROR_CODE)
	{
		retVal = ERROR_CODE;
		goto server_cleanup_3;
	}

	// start the CheckExit thread




	while (TRUE)
	{
	
		//start the accept thread 
	
		// wait for eather exit or client trying to connect

		// check Which thread has finished

		if(// EXIT)
			break;

	
	
	
	
	
	
	
	}

	/* EXIT procedure */





server_cleanup_3:
	//Close all semaphore and Handles
server_cleanup_2:
	if(closesocket(MainSocket) == SOCKET_ERROR)
		printf("Failed to close MainSocket, error %ld. Ending program\n", WSAGetLastError());

server_cleanup_1: //Close WINSOCK 
	if (WSACleanup() == SOCKET_ERROR)
		printf("Failed to close Winsocket, error %ld. Ending program.\n", WSAGetLastError());
	retrun ret; 
}