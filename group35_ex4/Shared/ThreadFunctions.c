// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 3 in the Course Introduction to system programming
// This is the file that include all the thread related functions

// Includes --------------------------------------------------------------------

#include "ThreadFunctions.h"
#include "SharedHardCodedData.h"

// Function Definitions --------------------------------------------------------

HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPDWORD p_thread_id, void *arg_pointer)
{
	HANDLE thread_handle;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread\n");
		printf("Received NULL pointer\n");
		return NULL;
	}

	if (NULL == p_thread_id)
	{
		printf("Error when creating a thread\n");
		printf("Received NULL pointer\n");
		return NULL;
	}

	thread_handle = CreateThread(
		NULL,            /*  default security attributes */
		0,               /*  use default stack size */
		p_start_routine, /*  thread function */
		arg_pointer,     /*  argument to thread function */
		0,               /*  use default creation flags */
		p_thread_id);    /*  returns the thread identifier */

	if (NULL == thread_handle)
	{
		printf("Couldn't create thread\n");
		return NULL;
	}

	return thread_handle;
}


int CloseThreads(HANDLE* p_thread_handles, int last_thread)
{
	int return_num = 0;
	int i = 0, ret_val = 0;
	// Close thread handles
	for (i = 0; i < last_thread; i++)
	{
		if (p_thread_handles[i] != NULL)
		{
			ret_val = CloseHandle(p_thread_handles[i]);
			if (false == ret_val)
			{
				printf("Error when closing thread handle\n");
				return_num = ERROR_CODE;
			}
		}
		else
		{
			printf("Error when closing thread handle\n");
			return_num = ERROR_CODE;
		}
	}
	return return_num;
}


