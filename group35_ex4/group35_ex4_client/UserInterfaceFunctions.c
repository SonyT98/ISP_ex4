
#include "UserInterfaceFunctions.h"

int ConnectionErrorMenu(int *user_pick, int connection_error_type, char *server_adr, char *server_port)
{
	int err = 0;
	char user_pick_s[MAX_LINE];
	int wrong_selection = 1;

	//if the connection failed
	if (connection_error_type == CONNECTION_FAILED)
		printf("Failed connecting to server on <%s>:<%s>.\n", server_adr, server_port);
	//if the connecion lost
	else if (connection_error_type == CONNECTION_LOST)
		printf("Connection to server on <%s>:<%s> has been lost.\n", server_adr, server_port);
	//the connection denied
	else
		printf("Server on <%s>:<%s> denied the connection request.\n", server_adr, server_port);

	while (wrong_selection)
	{
		//print the menu options
		printf("Choose what to do next:\n1. Try to reconnect\n2. Exit\n");
		err = scanf_s("%s", user_pick_s, MAX_LINE);
		if (err == 0 || err == EOF)
		{
			printf("Error: Reading from the console\n");
			return ERROR_CODE;
		}
		if (STRINGS_ARE_EQUAL(user_pick_s, "1"))
		{
			*user_pick = 1;
			wrong_selection = 0;
			break;
		}
		else if (STRINGS_ARE_EQUAL(user_pick_s, "2"))
		{
			*user_pick = 2;
			wrong_selection = 0;
			break;
		}
		else
		{
			printf("This Option is not avalibale or wrong.\nPlease select again.\n");
			wrong_selection = 1;
		}
	}

	return connection_error_type;
}

int MainMenuSelection(SOCKET sock, int *connect_again, char *server_adr, char *server_port, int *client_disconnected)
{
	//variables
	char message_type[MAX_MESSAGE];
	char message_send[MAX_MESSAGE];
	char message_info[MAX_MESSAGE];
	char user_pick_s[MAX_LINE];

	sendthread_s packet;


	int size_arr = 0, ret = 0, exit_code = 0, err = 0;

	packet.sock = sock;

	//menu selection for the user
	printf("Choose what to do next:\n");
	printf("1. Play against another client\n2. Play against the server\n3. View the leaderboard\n4. Quit\n");
	err = scanf_s("%s", user_pick_s, MAX_LINE);
	if (err == 0 || err == EOF)
	{
		printf("Error: Reading from the console\n");
		return ERROR_CODE;
	}

	//choose the main menu option
	if (STRINGS_ARE_EQUAL(user_pick_s, "1"))
		err = sprintf_s(message_send, MAX_MESSAGE, "%s\n", CLIENT_VERSUS);
	else if (STRINGS_ARE_EQUAL(user_pick_s, "2"))
		err = sprintf_s(message_send, MAX_MESSAGE, "%s\n", CLIENT_CPU);
	else if (STRINGS_ARE_EQUAL(user_pick_s, "3"))
		err = sprintf_s(message_send, MAX_MESSAGE, "%s\n", CLIENT_LEADERBOARD);
	else if (STRINGS_ARE_EQUAL(user_pick_s, "4"))
	{
		err = sprintf_s(message_send, MAX_MESSAGE, "%s\n", CLIENT_DISCONNECT);
		*client_disconnected = 1;
	}
	else
	{
		printf("Error: This Option is not avalibale or wrong\n");
		return ERROR_CODE;
	}
	if (err == 0 || err == EOF)
	{
		printf("Error: can't create the message for the client\n");
		return ERROR_CODE;
	}

	/*---------------------------- send client main menu pick -----------------------------*/

	packet.array_size = strlen(message_send);
	packet.array_t = message_send;

	//activate the send thread and get his exit code
	exit_code = ActivateThread((void*)&packet, 1, SENDRECV_WAITTIME);
	//if the thread setup failed or the thread function itself failed
	if (exit_code == ERROR_CODE) { return ERROR_CODE; }
	else if (exit_code != 0)
		return ConnectionErrorMenu(connect_again, CONNECTION_LOST, server_adr, server_port);

	return 0;
}

int GameResultDisplay()