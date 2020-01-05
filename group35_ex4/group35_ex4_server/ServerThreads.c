
#include "ServerThreads.h"


DWORD AcceptClientThread(LPSTR lpParam)
{
	SOCKET AcceptSocket = accept(MainSocket, NULL, NULL);
	if (AcceptSocket == INVALID_SOCKET)
	{
		printf("Accepting connection with client failed, error %ld\n", WSAGetLastError());
	}

}