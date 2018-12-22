#pragma once

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#define DEBUG_ON 1
#define CLIENT_MODE "Client"
#define SERVER_MODE "Server"
#define INPUT_MODE_FILE "file"
#define INPUT_MODE_HUMAN "human"

//message types from HW
#define NEW_UESR_REQUEST "NEW_UESR_REQUEST"
#define NEW_USER_ACCEPTED "NEW_USER_ACCEPTED"
#define NEW_USER_DECLINED "NEW_USER_DECLINED"
#define GAME_STARTED "GAME_STARTED"
#define BOARD_VIEW "BOARD_VIEW"
#define TURN_SWITCH "TURN_SWITCH"
#define PLAY_REQUEST "PLAY_REQUEST"
#define PLAY_ACCEPTED "PLAY_ACCEPTED"
#define PLAY_DECLINED "PLAY_DECLINED"
#define GAME_ENDED "GAME_ENDED"
#define SEND_MESSAGE "SEND_MESSAGE"
#define RECEIVE_MESSAGE "RECEIVE_MESSAGE"


FILE* debugFile ;
char* debugFilePath;
char* serverPort;
char* username;
char* inputMode;
char* inputFile;

SOCKET mainClientSocket;