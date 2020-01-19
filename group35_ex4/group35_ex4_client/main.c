
#include "ClientMainFunctions.h"
#include "UserInterfaceFunctions.h"


int main(int argc, char** argv)
{
	//variables
	int err = 0;
	// check number of input arguments
	if (argc != 4)
	{
		if (argc > 4) printf("Error: Too many arguments for Hw4\n");
		else printf("Error: Too few arguments for Hw4\n");
		return ERROR_CODE;
	}

	err = clientMain(argv[3],argv[1], argv[2]);
	return err;
}