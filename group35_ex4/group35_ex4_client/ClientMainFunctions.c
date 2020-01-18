
#include "ClientMainFunctions.h"

int clientMain(char *username, char *server_adr, char *server_port)
{
	//variables
	SOCKET client_sock = INVALID_SOCKET;
	int ret = 0, retVal = 0;
	int connect_again = 1;
	int replay = 1;
	int close_socket = 0;

	// Initialize Winsock.
	WSADATA wsaData;
	int StartupRes = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (StartupRes != NO_ERROR)
	{
		printf("error %ld at WSAStartup( ), ending program.\n", WSAGetLastError());                                
		return ERROR_CODE;
	}
	/* The WinSock DLL is acceptable. Proceed. */

	while (connect_again == 1)
	{
		//if there is a new connection, we want to close the socket
		if (close_socket == 1)
			closesocket(client_sock);

		//the first run the socket is close
		//any other runs on the while, will have the socket intialize again
		close_socket = 1;


		// Initialize client socket
		retVal = initializeConnection(&client_sock, server_adr, server_port, &connect_again);
		// if the connection it self failed exit the client
		if (retVal == ERROR_CODE)
		{ ret = ERROR_CODE; goto client_cleanup_1; }
		// connection failed and update the connection if necessery
		else if (retVal != 0)
			continue;

		//request connection from the server
		retVal = RequestConnection(client_sock, &connect_again, server_adr, server_port,username);
		// if the connection it self failed exit the client
		if (retVal == ERROR_CODE)
		{ ret = ERROR_CODE; goto client_cleanup_2; }
		// connection failed and update the connection if necessery
		else if (retVal != 0)
			continue; 

		while (TRUE)
		{

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
	int ret = 0;

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
		ret = ConnectionErrorMenu(connect_again, CONNECTION_FAILED, server_adr, server_port);
		if (ret == ERROR_CODE)
			closesocket(*sock);
		return ret;
	}

	return 0;

}

int RequestConnection(SOCKET sock, int *connect_again, char *server_adr, char *server_port, char *username)
{
	//variables
	char message_type[MAX_MESSAGE];
	char message_send[MAX_MESSAGE];
	char message_info[MAX_MESSAGE];

	sendthread_s packet;

	int size_arr = 0, ret = 0, exit_code = 0, err = 0;

	packet.sock = sock;

	/*----------------------------send CLIENT_REQUEST-----------------------------*/
	err = sprintf_s(message_send, MAX_MESSAGE, "%s:%s\n", CLIENT_REQUEST, username);
	if (err == 0 || err == EOF)
	{
		printf("Error: can't create the message for the client\n");
		ret = ERROR_CODE;
		goto return_ret;
	}
	packet.array_size = strlen(message_send);
	packet.array_t = message_send;

	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)&packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code == ERROR_CODE) { ret = ERROR_CODE;  goto return_ret; }
	//if the thread is on timeout or transsmition failed
	else if (exit_code != 0)
		return ConnectionErrorMenu(connect_again, CONNECTION_LOST, server_adr, server_port);


	/*----------------------------recv SERVER_APPROVED/SERVER_DENIED-----------------------------*/
	packet.array_t = NULL;
	packet.array_size = 0;

	//activate the recv thread and get his exit code
	exit_code = ActivateThread((void*)&packet, 0, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code == ERROR_CODE) { ret = ERROR_CODE;  goto return_ret; }
	else if (exit_code != 0)
		return ConnectionErrorMenu(connect_again, CONNECTION_LOST, server_adr, server_port);

	//get the message type and the information
	err = MessageCut(packet.array_t, packet.array_size, message_type, message_info);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto packet_cleanup; }

	//if server denied, send to connection error menu
	if (STRINGS_ARE_EQUAL(message_type, SERVER_DENIED))
	{
		ret = ConnectionErrorMenu(connect_again, CONNECTION_DENIED, server_adr, server_port);
		goto packet_cleanup;
	}
	else if (STRINGS_ARE_EQUAL(message_type, SERVER_APPROVED))
		goto packet_cleanup;
	else
	{
		printf("Error: message recieved from the server doesnt match with the protocol\n");
		ret = ERROR_CODE;
		goto packet_cleanup;
	}

packet_cleanup:
	free(packet.array_t);
return_ret:
	return ret;
}

int ReceiveMessageFromServer(SOCKET sock, int *connect_again, char *server_adr, char *server_port, int *client_disconnect)
{
	//variables
	char message_type[MAX_MESSAGE];
	char message_send[MAX_MESSAGE];
	char message_info[MAX_MESSAGE];

	sendthread_s packet;

	int size_arr = 0, ret = 0, exit_code = 0, err = 0;

	packet.sock = sock;

	/*----------------------------recv message-----------------------------*/
	packet.array_t = NULL;
	packet.array_size = 0;

	//activate the recv thread and get his exit code
	exit_code = ActivateThread((void*)&packet, 0, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code == ERROR_CODE) { ret = ERROR_CODE;  goto return_ret; }
	else if (exit_code != 0)
		return ConnectionErrorMenu(connect_again, CONNECTION_LOST, server_adr, server_port);

	//get the message type and the information
	err = MessageCut(packet.array_t, packet.array_size, message_type, message_info);
	if (err == ERROR_CODE) { ret = ERROR_CODE; goto packet_cleanup; }

	//the server send main menu selection
	if (STRINGS_ARE_EQUAL(message_type, SERVER_MAIN_MENU))
	{
		err = MainMenuSelection(sock, connect_again, server_adr, server_port, client_disconnect);
		if (err == ERROR_CODE) { ret = ERROR_CODE; goto packet_cleanup; }
		else if (err != 0)
			return ConnectionErrorMenu(connect_again, CONNECTION_LOST, server_adr, server_port);
	}
	// the server inform the client that a match going to start
	else if (STRINGS_ARE_EQUAL(message_type, SERVER_INVITE))
		goto packet_cleanup;
	//if the server ask the client to play his move
	else if (STRINGS_ARE_EQUAL(message_type, SERVER_PLAYER_MOVE_REQUEST))
	{

	}

packet_cleanup:
	free(packet.array_t);
return_ret:
	return ret;
}
