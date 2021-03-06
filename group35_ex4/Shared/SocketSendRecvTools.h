// Authors - Tomer Segal 207133646, Nadav Nave 209365725
// Exercise 4 in the Course Introduction to system programming
// The functions that handles the send and recevie tools on both projects

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/
/* 
 This file was written for instruction purposes for the 
 course "Introduction to Systems Programming" at Tel-Aviv
 University, School of Electrical Engineering, Winter 2011, 
 by Amnon Drory.
*/
/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#ifndef SOCKET_SEND_RECV_TOOLS_H
#define SOCKET_SEND_RECV_TOOLS_H

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include "SharedHardCodedData.h"

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

typedef enum { TRNS_FAILED, TRNS_DISCONNECTED, TRNS_SUCCEEDED } TransferResult_t;

/**
 * SendBuffer() uses a socket to send a buffer.
 *
 * Accepts:
 * -------
 * Buffer - the buffer containing the data to be sent.
 * BytesToSend - the number of bytes from the Buffer to send.
 * sd - the socket used for communication.
 *
 * Returns:
 * -------
 * TRNS_SUCCEEDED - if sending succeeded
 * TRNS_FAILED - otherwise
 */
TransferResult_t SendBuffer( const char* Buffer, int BytesToSend, SOCKET sd );

/**
 * SendString() uses a socket to send a string.
 * Str - the string to send. 
 * sd - the socket used for communication.
 */ 
TransferResult_t SendCharArray(const char *Str, SOCKET sd, const int *ArraySize);

/**
 * Accepts:
 * -------
 * ReceiveBuffer() uses a socket to receive a buffer.
 * OutputBuffer - pointer to a buffer into which data will be written
 * OutputBufferSize - size in bytes of Output Buffer
 * BytesReceivedPtr - output parameter. if function returns TRNS_SUCCEEDED, then this 
 *					  will point at an int containing the number of bytes received.
 * sd - the socket used for communication.
 *
 * Returns:
 * -------
 * TRNS_SUCCEEDED - if receiving succeeded
 * TRNS_DISCONNECTED - if the socket was disconnected
 * TRNS_FAILED - otherwise
 */ 
TransferResult_t ReceiveBuffer( char* OutputBuffer, int RemainingBytesToReceive, SOCKET sd );

/**
 * ReceiveString() uses a socket to receive a string, and stores it in dynamic memory.
 * 
 * Accepts:
 * -------
 * OutputStrPtr - a pointer to a char-pointer that is initialized to NULL, as in:
 *
 *		char *Buffer = NULL;
 *		ReceiveString( &Buffer, ___ );
 *
 * a dynamically allocated string will be created, and (*OutputStrPtr) will point to it.
 * 
 * sd - the socket used for communication.
 * 
 * Returns:
 * -------
 * TRNS_SUCCEEDED - if receiving and memory allocation succeeded
 * TRNS_DISCONNECTED - if the socket was disconnected
 * TRNS_FAILED - otherwise
 */ 
TransferResult_t ReceiveCharArray(char** OutputStrPtr, SOCKET sd, int *ArraySize);

/*
* SendThread will send the buffer to the specific socket.
* Input Argument:
*	lpParam - global paramter to the thread, when we use this function
*	we will send the sendthread_s struct.
* Output:
* return 0x555 if the message didnt got delivered.
*/
DWORD SendThread(LPSTR lpParam);

/*
* RecvThread will receive the buffer from specific socket
* Input Argument:
*	lpParam - global paramter to the thread, when we use this function
*	we will send the sendthread_s struct.
* Output:
* return 0x555 if the message didnt go through. return 1 if there is a
* disconnection.
*/
DWORD RecvThread(LPSTR lpParam);

/*
* ActivateThread gets a null handle for the thread and activate the specific thread
* for sending of receving a message.
* Input Arguments:
*	arg			  - an argument for the thread function.
*	recv_or_send  - zero if we recevie and 1 if we send.
*	waittime	  - the wait time of the thread.
* Output:
*	return -1 if failed, or the exit code of the thread otherwise.
*/
int ActivateThread(void *arg, int recv_or_send, int waittime);

/*
* Message_cut cutts the received message and to its message type
* and message information as strings.
* Input Arguments:
*	message - the whole message the we recevied.
*	max_size - the message size.
*	message_type - the message type that we want to update;
*	info - the information message we want to update;
* Output:
*	return -1 if the message isn't right.
*/
int MessageCut(char *message, int message_size, char* message_type, char *info);

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#endif // SOCKET_SEND_RECV_TOOLS_H