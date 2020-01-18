
#include "ClientMainFunctions.h"

int clientMain(char *username, char *server_adr, char *server_port)
{
	//variables
	SOCKET client_sock = INVALID_SOCKET;
	int ret = 0, retVal = 0;
	int connect_again = 1;

	// Initialize Winsock.
	WSADATA wsaData;
	int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (StartupRes != NO_ERROR)
	{
		printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());                                
		return ERROR_CODE;
	}
	/* The WinSock DLL is acceptable. Proceed. */

	while (connect_again)
	{
		/* Initialize client socket */
		retVal = initializeConnection(&client_sock, server_adr, server_port, &connect_again);
		if (retVal == ERROR_CODE)
		{
			ret = ERROR_CODE;
			goto client_cleanup_1;
		}
	}
client_cleanup_2:
	closesocket(client_sock);
client_cleanup_1:
	WSACleanup();
	return ret;
}

int initializeConnection(SOCKET *sock, char *server_adr, char *server_port, int *connect_again)
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
	clientService.sin_addr.s_addr = inet_addr(server_adr); //Setting the IP address to connect to
	clientService.sin_port = htons(atoi(server_port)); //Setting the port to connect to.



	// Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
	// Check for general errors.
	if (connect(*sock, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) 
	{
		// the connection lost, and we need to get the user pick
		printf("Failed to connect.\n");
		closesocket(*sock);
		return CONNECTION_FAILED;
	}

	return 0;

}

int ConnectionErrorMenu(int *user_pick, int connection_error_type, char *server_adr, char *server_port)
{
	//if the connection failed
	if (connection_error_type == CONNECTION_FAILED)
		printf("Failed connecting to server on <%s>:<%s>.", server_adr, server_port);
	//if the connecion lost
	else if (connection_error_type == CONNECTION_LOST)
		printf("Connection to server on <%s>:<%s> has been lost", server_adr, server_port);
}
