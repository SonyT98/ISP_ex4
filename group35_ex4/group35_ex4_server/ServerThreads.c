
#include "ServerThreads.h"
#include "SharedHardCodedData.h"
#include "ServiceFunctions.h"


DWORD WINAPI AcceptClientThread(LPSTR lpParam)
{
	AcceptSocketParams *param = (AcceptSocketParams*)lpParam;

	
	param->AcceptSocket = accept(param->mainSocket , NULL, NULL);
	if (param->AcceptSocket == INVALID_SOCKET)
	{
		printf("Accepting connection with client failed, error %ld\n", WSAGetLastError());
		return ERROR_CODE;
	}
	return 1;
}

DWORD WINAPI CheckExitThread(LPSTR lpParam)
{
	bool flag = TRUE;
	char message[100];
	int retVal = 0;
	while (flag)
	{
		retVal = scanf_s("%s", message,100);

		if (strcmp(message, "exit") == 0)
		{
			return 2;
		}
	}
	return ERROR_CODE;
}

DWORD WINAPI ServiceThread(LPSTR lpParam)
{
	ServiceThreadParams *arg = NULL;

	arg = (ServiceThreadParams*)lpParam;

	//variables
	char username[MAX_USERNAME];
	char player_move[MAX_MESSAGE];
	char cpu_move[MAX_MESSAGE];
	int err = 0;
	int menu_selection = 0, winning_player = 0, replay = 0;

	//get the client username and accept him
	err = ClientUsername(arg->client_socket, username);
	
	while (!replay)
	{
		//get the main menu selection from the client
		err = SelectFromMenu(arg->client_socket, &menu_selection);

		switch (menu_selection)
		{
		case 1:
			err = CPUGame(arg->client_socket, player_move, cpu_move, &winning_player);
			err = EndGameStatus(arg->client_socket, username, "Server", player_move, cpu_move, winning_player, &replay);
			break;
		}
	}
}