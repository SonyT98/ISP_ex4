/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
 This file was written for instruction purposes for the 
 course "Introduction to Systems Programming" at Tel-Aviv
 University, School of Electrical Engineering, Winter 2011, 
 by Amnon Drory, based on example code by Johnson M. Hart.
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include "SocketSendRecvTools.h"
#include "SharedHardCodedData.h"


/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

TransferResult_t SendBuffer( const char* Buffer, int BytesToSend, SOCKET sd )
{
	const char* CurPlacePtr = Buffer;
	int BytesTransferred;
	int RemainingBytesToSend = BytesToSend;
	
	while ( RemainingBytesToSend > 0 )  
	{
		/* send does not guarantee that the entire message is sent */
		BytesTransferred = send (sd, CurPlacePtr, RemainingBytesToSend, 0);
		if ( BytesTransferred == SOCKET_ERROR ) 
		{
			printf("send() failed, error %d\n", WSAGetLastError() );
			return TRNS_FAILED;
		}
		
		RemainingBytesToSend -= BytesTransferred;
		CurPlacePtr += BytesTransferred; // <ISP> pointer arithmetic
	}

	return TRNS_SUCCEEDED;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

TransferResult_t SendCharArray( const char *Str, SOCKET sd, int* ArraySize)
{
	/* Send the the request to the server on socket sd */
	TransferResult_t SendRes;

	/* The request is sent in two parts. First the Length of the string (stored in 
	   an int variable ), then the string itself. */

	SendRes = SendBuffer( 
		(const char *)( ArraySize ),
		(int)( sizeof(int) ), // sizeof(int) 
		sd );

	if ( SendRes != TRNS_SUCCEEDED ) return SendRes ;

	SendRes = SendBuffer( 
		(const char *)( Str ),
		(int)( *ArraySize ), 
		sd );

	return SendRes;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

TransferResult_t ReceiveBuffer( char* OutputBuffer, int BytesToReceive, SOCKET sd )
{
	char* CurPlacePtr = OutputBuffer;
	int BytesJustTransferred;
	int RemainingBytesToReceive = BytesToReceive;
	
	while ( RemainingBytesToReceive > 0 )  
	{
		/* send does not guarantee that the entire message is sent */
		BytesJustTransferred = recv(sd, CurPlacePtr, RemainingBytesToReceive, 0);
		if ( BytesJustTransferred == SOCKET_ERROR ) 
		{
			printf("recv() failed, error %d\n", WSAGetLastError() );
			return TRNS_FAILED;
		}		
		else if ( BytesJustTransferred == 0 )
			return TRNS_DISCONNECTED; // recv() returns zero if connection was gracefully disconnected.

		RemainingBytesToReceive -= BytesJustTransferred;
		CurPlacePtr += BytesJustTransferred; // <ISP> pointer arithmetic
	}

	return TRNS_SUCCEEDED;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

TransferResult_t ReceiveCharArray( char** OutputStrPtr, SOCKET sd, int *ArraySize )
{
	/* Recv the the request to the server on socket sd */
	int TotalArraySizeInBytes;
	TransferResult_t RecvRes;
	char* CharBuffer = NULL;

	if ( ( OutputStrPtr == NULL ) || ( *OutputStrPtr != NULL ) )
	{
		printf("The first input to ReceiveString() must be " 
			   "a pointer to a char pointer that is initialized to NULL. For example:\n"
			   "\tchar* Buffer = NULL;\n"
			   "\tReceiveString( &Buffer, ___ )\n" );
		return TRNS_FAILED;
	}

	/* The request is received in two parts. First the Length of the string (stored in 
	   an int variable ), then the string itself. */
		
	RecvRes = ReceiveBuffer( 
		(char *)( &TotalArraySizeInBytes ),
		(int)( sizeof(TotalArraySizeInBytes) ), // 4 bytes
		sd );

	if ( RecvRes != TRNS_SUCCEEDED ) return RecvRes;

	CharBuffer = (char*)malloc( TotalArraySizeInBytes * sizeof(char) );

	if ( CharBuffer == NULL )
		return TRNS_FAILED;

	RecvRes = ReceiveBuffer( 
		(char *)( CharBuffer ),
		(int)( TotalArraySizeInBytes), 
		sd );

	if ( RecvRes == TRNS_SUCCEEDED ) 
		{ *OutputStrPtr = CharBuffer; }
	else
	{
		free( CharBuffer );
	}
	*ArraySize = TotalArraySizeInBytes;
		
	return RecvRes;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

DWORD WINAPI SendThread(LPSTR lpParam)
{
	sendthread_s *arg;
	TransferResult_t SendRes;

	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		return ERROR_CODE;
	}

	arg = (sendthread_s*)lpParam;
	SendRes = SendCharArray(arg->array_t, arg->sock, &(arg->array_size));

	if (SendRes == TRNS_FAILED)
	{
		printf("Socket error while trying to write data to socket\n");
		return 0x555;
	}
	return 0;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

DWORD WINAPI RecvThread(LPSTR lpParam)
{
	sendthread_s *arg;
	TransferResult_t RecvRes;

	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		return ERROR_CODE;
	}

	arg = (sendthread_s*)lpParam;
	RecvRes = ReceiveCharArray(&(arg->array_t), arg->sock, &(arg->array_size));
	if (RecvRes == TRNS_FAILED)
	{
		printf("Service socket error while reading\n");
		return 0x555;
	}
	else if (RecvRes == TRNS_DISCONNECTED)
	{
		printf("Connection closed while reading\n");
		return CONNECTION_DISCONNECTED;
	}
	return 0;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

int ActivateThread(void *arg, int recv_or_send, int waittime)
{
	HANDLE thread_handle = NULL;
	LPWORD thread_id;

	int ret = 0, wait_ret = 0, err_flag = 0;
	DWORD exit_code = 0;

	if (recv_or_send == 0)
		//call the recevie thread
		thread_handle = CreateThreadSimple((LPTHREAD_START_ROUTINE)RecvThread, &thread_id, arg);
	else
		//call the send thread
		thread_handle = CreateThreadSimple((LPTHREAD_START_ROUTINE)SendThread, &thread_id, arg);

	//create thread simple failed
	if (thread_handle == NULL)
	{
		printf("Error creating RecvThread\n");
		ret = ERROR_CODE;
		goto return_goto;
	}

	//wait for the thread with timeout of 15 seconds
	wait_ret = WaitForSingleObject(thread_handle, waittime);

	//if there is a timeout, terminate the thread and close the thread
	if (wait_ret == WAIT_TIMEOUT)
	{
		printf("Error: wait timeout to send the message\n");
		printf("Terminating thread and exiting\n");
		err_flag = TerminateThread(thread_handle, exit_code);
		ret = WAIT_TIMEOUT;
		goto main_cleanup1;
	}
	//the wait for signle object failed, close the thread
	else if (wait_ret != WAIT_OBJECT_0)
	{
		printf("Error while waiting using WaitForSingleObject\n");
		ret = ERROR_CODE;
		goto main_cleanup1;
	}

	//if the thread is runnig correctly, get the exit code
	err_flag = GetExitCodeThread(thread_handle, &exit_code);
	if (err_flag == 0)
	{
		printf("Error while getting exit code from thread\n");
		ret = ERROR_CODE;
		goto main_cleanup1;
	}

	ret = exit_code;


main_cleanup1:
	CloseHandle(thread_handle);
return_goto:
	return ret;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

int MessageCut(char *message, int message_size, char* message_type, char *info)
{
	int i = 0;
	int info_or_type = 0;
	int type_size = 0;

	for (i = 0; i < message_size; i++)
	{
		//if this is a message type
		if (info_or_type == 0)
		{
			message_type[i] = message[i];
			type_size = i + 1;
			if (message_type[i] == ':')
			{
				message_type[i] = '\0';
				info_or_type = 1;
			}
		}
		//this is a info type
		else
			info[i - type_size] = message[i];
	}
	//if we didnt got to ':' char this message has no info
	if (info_or_type == 0)
	{
		info[0] = '\0';
		return 0;
	}
	// set info as string
	else
		info[i - type_size - 1] = '\0';
	return 0;
}