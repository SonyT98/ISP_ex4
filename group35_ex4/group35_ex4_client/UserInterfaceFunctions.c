
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

int MainMenuSelection(SOCKET sock, int *main_menu_selection)
{
	//variables
	char message_send[MAX_MESSAGE];
	char user_pick_s[MAX_LINE];

	sendthread_s packet;

	int exit_code = 0, err = 0;

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
	{
		err = sprintf_s(message_send, MAX_MESSAGE, "%s\n", CLIENT_VERSUS);
		*main_menu_selection = VERSUS_GAME_SELECTION;
	}
	else if (STRINGS_ARE_EQUAL(user_pick_s, "2"))
	{
		err = sprintf_s(message_send, MAX_MESSAGE, "%s\n", CLIENT_CPU);
		*main_menu_selection = CPU_GAME_SELECTION;
	}
	else if (STRINGS_ARE_EQUAL(user_pick_s, "3"))
	{
		err = sprintf_s(message_send, MAX_MESSAGE, "%s\n", CLIENT_LEADERBOARD);
		*main_menu_selection = LEADERBOARD_SELECTION;
	}
	else if (STRINGS_ARE_EQUAL(user_pick_s, "4"))
	{
		err = sprintf_s(message_send, MAX_MESSAGE, "%s\n", CLIENT_DISCONNECT);
		*main_menu_selection = DISCONNECT_SELECTION;
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
	if (exit_code == ERROR_CODE)  return ERROR_CODE; 
	else if (exit_code != 0)  return CONNECTION_LOST; 

	return 0;
}

int PlayerMoveRequest(SOCKET sock)
{
	//variables
	char message_send[MAX_MESSAGE];
	char user_pick_s[MAX_LINE];

	sendthread_s packet;

	int exit_code = 0, err = 0;

	packet.sock = sock;
	//ask the ser to pick his move
	printf("Choose a move from the list: Rock, Paper, Scissors, Lizard or Spock:\n");
	err = scanf_s("%s", user_pick_s, MAX_LINE);
	if (err == 0 || err == EOF)
	{
		printf("Error: Reading from the console\n");
		return ERROR_CODE;
	}
	StringUpper(user_pick_s);


	//choose the player pick
	if (STRINGS_ARE_EQUAL(user_pick_s, "SPOCK"))
		err = sprintf_s(message_send, MAX_MESSAGE, "%s:%s\n",CLIENT_PLAYER_MOVE, "SPOCK");
	else if (STRINGS_ARE_EQUAL(user_pick_s, "ROCK"))
		err = sprintf_s(message_send, MAX_MESSAGE, "%s:%s\n", CLIENT_PLAYER_MOVE, "ROCK");
	else if (STRINGS_ARE_EQUAL(user_pick_s, "PAPER"))
		err = sprintf_s(message_send, MAX_MESSAGE, "%s:%s\n", CLIENT_PLAYER_MOVE, "PAPER");
	else if (STRINGS_ARE_EQUAL(user_pick_s, "LIZARD"))
		err = sprintf_s(message_send, MAX_MESSAGE, "%s:%s\n", CLIENT_PLAYER_MOVE, "LIZARD");
	else if (STRINGS_ARE_EQUAL(user_pick_s, "SCISSORS"))
		err = sprintf_s(message_send, MAX_MESSAGE, "%s:%s\n", CLIENT_PLAYER_MOVE, "SCISSORS");
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
	if (exit_code == ERROR_CODE)  return ERROR_CODE;
	else if (exit_code != 0)  return CONNECTION_LOST;

	return 0;
}

void StringUpper(char *a)
{
	int i = 0;
	while (a[i] != '\0')
	{
		a[i] = (char)toupper(a[i]);
		i++;
	}
}




int GameOverMenu(SOCKET sock)
{
	char message_send[MAX_MESSAGE];
	char user_pick_s[MAX_LINE];

	int user_pick_i = -1;
	sendthread_s packet;

	int exit_code = 0, err = 0;

	packet.sock = sock;

	//menu selection for the user
	printf("Choose what to do next:\n");
	printf("1. Play again\n2. Return to the main menu\n");
	err = scanf_s("%s", user_pick_s, MAX_LINE);
	if (err == 0 || err == EOF)
	{
		printf("Error: Reading from the console\n");
		return ERROR_CODE;
	}
	// check the user pick
	if (STRINGS_ARE_EQUAL(user_pick_s, "1") || STRINGS_ARE_EQUAL(user_pick_s, "2"))
	{
		err = sprintf_s(message_send, MAX_MESSAGE, "%s\n", CLIENT_VERSUS);
	}
	else
	{
		printf("Error: This Option is not available or wrong\n");
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
	if (exit_code == ERROR_CODE)  return ERROR_CODE;
	else if (exit_code != 0)  return CONNECTION_LOST;

	return 0;

}