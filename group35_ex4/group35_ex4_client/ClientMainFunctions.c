
#include "ClientMainFunctions.h"

int clientMain()
{
	HANDLE hThread[2];
	SOCKET client_sock = INVALID_SOCKET;
	int ret = 0, retVal = 0;

	char sendMessage[MAX_MESSAGE] = {NULL};
	char recvMessage[MAX_MESSAGE] = "";

	char messageType[MAX_MESSAGE] = "";
	char messageInfo[MAX_MESSAGE] = "";

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
	retVal = sprintf_s(sendMessage,sizeof(sendMessage),"%s", "CLIENT_REQUEST:NADAVNAVE\n");
	send.array_size = MAX_MESSAGE;
	send.array_t = sendMessage;
	send.sock = client_sock;
	retVal = ActivateThread((void*) &send, 1, INFINITE);

	// get ack
	recive.array_size = MAX_MESSAGE;
	recive.array_t = recvMessage;
	recive.sock = client_sock;
	retVal = ActivateThread((void*)&recive, 0, INFINITE);
	if (retVal != 0) { goto client_cleanup_2; }
	retVal = MessageCut(recive.array_t, recive.array_size, messageType, messageInfo);

	// get main menu
	recive.array_size = MAX_MESSAGE;
	recive.array_t = recvMessage;
	recive.sock = client_sock;
	retVal = ActivateThread((void*)&recive, 0, INFINITE);
	if (retVal != 0) { goto client_cleanup_2; }
	retVal = MessageCut(recive.array_t, recive.array_size, messageType, messageInfo);

	// pick what to do
	retVal = sprintf_s(sendMessage,sizeof(sendMessage),"%s", "CLIENT_CPU:\n");
	send.array_size = MAX_MESSAGE;
	send.array_t = sendMessage;
	send.sock = client_sock;
	retVal = ActivateThread((void*)&send, 1, INFINITE);

	// get move
	recive.array_size = MAX_MESSAGE;
	recive.array_t = recvMessage;
	recive.sock = client_sock;
	retVal = ActivateThread((void*)&recive, 0, INFINITE);
	if (retVal != 0) { goto client_cleanup_2; }
	retVal = MessageCut(recive.array_t, recive.array_size, messageType, messageInfo);

	// pick what to do
	retVal = sprintf_s(sendMessage, sizeof(sendMessage), "%s", "CLIENT_PLAYER_MOVE:ROOK\n");
	send.array_size = MAX_MESSAGE;
	send.array_t = sendMessage;
	send.sock = client_sock;
	retVal = ActivateThread((void*)&send, 1, INFINITE);

	// get results
	recive.array_size = MAX_MESSAGE;
	recive.array_t = recvMessage;
	recive.sock = client_sock;
	retVal = ActivateThread((void*)&recive, 0, INFINITE);
	if (retVal != 0) { goto client_cleanup_2; }
	retVal = MessageCut(recive.array_t, recive.array_size, messageType, messageInfo);


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