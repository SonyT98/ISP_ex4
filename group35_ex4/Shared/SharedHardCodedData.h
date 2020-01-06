#pragma once
#ifndef __SHAREDHARDCODEDDATA_H__
#define __SHAREDHARDCODEDDATA_H___

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <limits.h>
#include <string.h>
#include <tchar.h>
#include <stdlib.h>

#define ERROR_CODE ((int)(-1))
#define MAX_USERNAME 20

#define SERVER_ADDRESS_STR "127.0.0.1"
#define SERVER_PORT 8888

#define STRINGS_ARE_EQUAL( Str1, Str2 ) ( strcmp( (Str1), (Str2) ) == 0 )

#endif // __HARDCODEDDATA_H___