
#include "ServerThreads.h"
#include "ThreadFunctions.h"
#include "HardCodedData.h"
#include "SharedHardCodedData.h"



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

int GetClientName(char *username)
{

}