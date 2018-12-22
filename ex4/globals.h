#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#define DEBUG_ON 1
#define CLIENT_MODE "Client"
#define SERVER_MODE "Server"
#define INPUT_MODE_FILE "file"
#define INPUT_MODE_HUMAN "human"


FILE* debugFile = NULL;
char* debugFilePath;
char* serverPort;
char* username;
char* inputMode;
char* inputFile;