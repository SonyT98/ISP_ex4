
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
	char *username = usernames[arg->index];

	char player_move[MAX_MESSAGE];
	char cpu_move[MAX_MESSAGE];
	int err = 0;
	int menu_selection = 0, winning_player = 0, replay = 1;
	int replay_choice = 0;
	int ret_val = 0;
	//get the client username and accept him
	err = ClientUsername(arg->client_socket, username);
	if (err == ERROR_CODE) return ERROR_CODE;

	while (TRUE)
	{

		//get the main menu selection from the client
		err = SelectFromMenu(arg->client_socket, &menu_selection);
		if (err == ERROR_CODE) return ERROR_CODE;

		while(replay == 1)
		{ 
			switch (menu_selection)
			{
			case 1: // Play with the server
				err = CPUGame(arg->client_socket, player_move, cpu_move, &winning_player);
				{
					closesocket(arg->client_socket);
					return ERROR_CODE;
				}
				err = EndGameStatus(arg->client_socket, username, "Server", player_move, cpu_move, winning_player, &replay);
				{
					closesocket(arg->client_socket);
					return ERROR_CODE;
				}
				break;
			case 2: // Play versus another player

				err = VersusGame(arg->client_socket,arg->index, player_move, cpu_move, &winning_player);
				{
					closesocket(arg->client_socket);
					return ERROR_CODE;
				}
				err = EndGameStatus(arg->client_socket, username, usernames[!arg->index], player_move, cpu_move, winning_player, &replay_choice);
				{
					closesocket(arg->client_socket);
					return ERROR_CODE;
				}
				replay = VersusReplayOptionCheck(replay_choice,arg->index);
				if (replay == ERROR_CODE)
				{
					closesocket(arg->client_socket);
					return ERROR_CODE;
				}
				break;
			case 3: // Leaderboard
				replay = 0;
				break;
			case 4:// Exit
				closesocket(arg->client_socket);
				return 0;

			}
		}
	}
}