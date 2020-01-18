
#include "ClientMainFunctions.h"



int main(int argc, char** argv)
{
	//variables
	char *username = NULL;
	char *server_adr = NULL;
	char *server_port = NULL;
	int err = 0;
	// check number of input arguments
	if (argc != 4)
	{
		if (argc > 4) printf("Error: Too many arguments for Hw3\n");
		else printf("Error: Too few arguments for Hw3\n");
		return ERROR_CODE;
	}

	username = argv[3];
	server_adr = argv[1];
	server_port = argv[2];

	return clientMain(username,server_adr,server_port);
}