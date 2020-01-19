
#include "ClientMainFunctions.h"
#include "UserInterfaceFunctions.h"


int main(int argc, char** argv)
{
	//variables

	int err = 0;
	// check number of input arguments
	//if (argc != 4)
	//{
	//	if (argc > 4) printf("Error: Too many arguments for Hw3\n");
	//	else printf("Error: Too few arguments for Hw3\n");
	//	return ERROR_CODE;
	//}



	int user_pick;
	int connection_error_type = 0;
	char server_adr[100] = "127.0.0.1";
	char server_port[100] = "8887";
	SOCKET sock = NULL;
	int connect_again = 0;
	int client_disconnected = 0;
	err = MainMenuSelection(sock, connect_again, server_adr, server_port, client_disconnected);
	return 0;
}