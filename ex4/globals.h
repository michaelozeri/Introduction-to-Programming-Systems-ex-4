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
#include <stdbool.h>
#include <WS2tcpip.h>

#define DEBUG_ON 1
#define CLIENT_MODE "Client"
#define SERVER_MODE "Server"
#define INPUT_MODE_FILE "file"
#define INPUT_MODE_HUMAN "human"
#define SEND_BUFFER_SIZE 300
#define LOCALHOST_IP_ADRESS "127.0.0.1"

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

//globals from printBoard.c
#define RED_PLAYER 1
#define YELLOW_PLAYER 2
#define BOARD_HEIGHT 6
#define BOARD_WIDTH  7
#define BLACK  15
#define RED    204
#define YELLOW 238


FILE* debugFile ;
char* debugFilePath;
int serverPort;
char* username;
char* inputMode;
char* inputFile;
SOCKET mainClientSocket;
char userBuffer[SEND_BUFFER_SIZE];
int MyUserNumber;