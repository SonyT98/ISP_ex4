
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

	char server_adr[100] = "127.0.0.1";
	char server_port[100] = "8887";
	char username[MAX_USERNAME] = "Nadav";
	err = clientMain(username, server_adr, server_port);
	return err;
}