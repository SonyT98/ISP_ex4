
#include "ClientMainFunctions.h"

int clientMain()
{
	HANDLE hThread[2];
	SOCKET client_sock = INVALID_SOCKET;
	int ret = 0, retVal = 0;

	char sendMassage[MAX_MESSAGE] = {NULL};
	char recvMassage[MAX_MESSAGE] = "";

	char massageType[MAX_MESSAGE] = "";
	char massageInfo[MAX_MESSAGE] = "";

	sendthread_s send;
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


	/* Initialize client socket */
	retVal = initializeConnection(&client_sock);
	if (retVal == ERROR_CODE)
	{
		ret = ERROR_CODE;
		goto client_cleanup_1;
	}


	// Send and receive data.
	retVal = sprintf_s(sendMassage,sizeof(sendMassage),"%s", "CLIENT_REQUEST:nadavnave\n");
	send.array_size = MAX_MESSAGE;
	send.array_t = sendMassage;
	send.sock = client_sock;
	retVal = ActivateThread((void*) &send, 1, INFINITE);
	// get ack
	recive.array_size = MAX_MESSAGE;
	recive.array_t = sendMassage;
	recive.sock = client_sock;
	retVal = ActivateThread((void*)&recive, 0, INFINITE);
	if (retVal != 0) { goto client_cleanup_2; }
	retVal = 
	// get main menu
	recive.array_size = MAX_MESSAGE;
	recive.array_t = sendMassage;
	recive.sock = client_sock;
	retVal = ActivateThread((void*)&recive, 0, INFINITE);

	// pick choice
	retVal = sprintf_s(sendMassage,sizeof(sendMassage),"%s", "CLIENT_REQUEST:nadavnave\n");
	send.array_size = MAX_MESSAGE;
	send.array_t = sendMassage;
	send.sock = client_sock;
	retVal = ActivateThread((void*)&send, 1, INFINITE);

client_cleanup_2:
	closesocket(client_sock);

client_cleanup_1:
	WSACleanup();

	return ret;
}

int initializeConnection(SOCKET *sock)
{

	SOCKADDR_IN clientService;


	// Create a socket.
	*sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Check for errors to ensure that the socket is a valid socket.
	if (*sock == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		return ERROR_CODE;
	}
	/*
	 The parameters passed to the socket function can be changed for different implementations.
	 Error detection is a key part of successful networking code.
	 If the socket call fails, it returns INVALID_SOCKET.
	 The if statement in the previous code is used to catch any errors that may have occurred while creating
	 the socket. WSAGetLastError returns an error number associated with the last error that occurred.
	 */


	 //For a client to communicate on a network, it must connect to a server.
	 // Connect to a server.

	 //Create a sockaddr_in object clientService and set  values.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(SERVER_ADDRESS_STR); //Setting the IP address to connect to
	clientService.sin_port = htons(SERVER_PORT); //Setting the port to connect to.

	/*
		AF_INET is the Internet address family.
	*/


	// Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
	// Check for general errors.
	if (connect(*sock, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		printf("Failed to connect.\n");
		closesocket(*sock);
		return ERROR_CODE;
	}

	return 0;

}