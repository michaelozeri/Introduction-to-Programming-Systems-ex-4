#pragma once

#include "Utility.h"

int debugToFile(char * log_msg)
{
	if (!DEBUG_ON) {
		return 0;
	}
	if (LOG_ALSO_TO_CONSOLE) {
		printf("%s\n", log_msg);
	}
	int msgLen = strlen(log_msg);
	errno_t err;
	int retval;
	if (debugFile == NULL) {
		err = fopen_s(&debugFile, debugFilePath, "a"); //open with append to write to end of file
		if (err)
		{
			//TODO: finish here
			printf("Error opening file!\n");
			exit(1);
		}
	}
	retval = fprintf(debugFile, "%s\n", log_msg);
	if (retval != msgLen) {
		//TODO: finish here
		printf("Error Writing to file!\n");
		exit(1);
	}
	//fclose(debugFile); close only on end of main
	return 0;
}

int sendMsg(char * msgType, char * params)
{
	int retVal;
	TransferResult_t transferResult;
	char buffer[SEND_BUFFER_SIZE];
	retVal = sprintf_s(buffer, SEND_BUFFER_SIZE, "%s:%s", msgType, params);
	if (retVal < 0) {
		printf("ERROR: failed writing into send buffer\n");
		return -1;
	}
	transferResult = SendBuffer(buffer, retVal, mainClientSocket);
	if (transferResult != TRNS_SUCCEEDED) {
		printf("ERROR: failed to send message\n");
		return -1;
	}
	return 0;
}

int sendMsg2Param(char * msgType, char * param1, char * param2)
{
	int retVal;
	char param1new[SEND_BUFFER_SIZE];
	retVal = sprintf_s(param1new, SEND_BUFFER_SIZE, "%s;%s", param1, param2);
	if (retVal < 0) {
		printf("ERROR: failed writing into send buffer\n");
		return -1;
	}
	return sendMsg(msgType, param1new);
}

int sendMsg3Param(char * msgType, char * param1, char * param2, char * param3)
{
	int retVal;
	char param2new[SEND_BUFFER_SIZE];
	retVal = sprintf_s(param2new, SEND_BUFFER_SIZE, "%s;%s", param2, param3);
	if (retVal < 0) {
		printf("ERROR: failed writing into send buffer\n");
		return -1;
	}
	return sendMsg2Param(msgType, param1, param2new);
}

int recvMsg(char ** inputBuffer)
{
	ReceiveString(inputBuffer, mainClientSocket);
	return 0;
}

int recvUserNumber(char * inputBuffer)
{
	char** messageAndParams;
	recvMsg(&inputBuffer);
	split(inputBuffer, ':', &messageAndParams);//split messege and params
	if (strcmp(messageAndParams[0], NEW_USER_ACCEPTED)) {
		if (strcmp(messageAndParams[0], NEW_USER_DECLINED)) {
			debugToFile("ERROR: expected user acccepted / declined");
			return -1;
		}
		else {
			debugToFile("ERROR: Request to join was refused\n");
			return -1;
		}
	}
	return atoi(messageAndParams[1]);
}

bool recvPlayAccepted(char * inputBuffer)
{
	char** messageAndParams;
	recvMsg(&inputBuffer);
	split(inputBuffer, ':', &messageAndParams);//split messege and params
	if (!strcmp(messageAndParams[0], PLAY_ACCEPTED)) {
		debugToFile("Well played\n");
		return true;
	}
	if (!strcmp(messageAndParams[0], PLAY_DECLINED)) { //TODO: check iilegal move and retray if msg == iilegal move
		char msg[SEND_BUFFER_SIZE];
		sprintf_s(msg, SEND_BUFFER_SIZE, "Error: %s",messageAndParams[1]);
		debugToFile(msg);
		return false;
	}
	
	return false;
}

void PrintBoard(int board[BOARD_HEIGHT][BOARD_WIDTH])
{
	//This handle allows us to change the console's color
	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int row, column;

	//Draw the board
	for (row = 0; row < BOARD_HEIGHT; row++)
	{
		for (column = 0; column < BOARD_WIDTH; column++)
		{
			printf("| ");
			if (board[row][column] == RED_PLAYER)
				SetConsoleTextAttribute(hConsole, RED);

			else if (board[row][column] == YELLOW_PLAYER)
				SetConsoleTextAttribute(hConsole, YELLOW);

			printf("O");

			SetConsoleTextAttribute(hConsole, BLACK);
			printf(" ");
		}
		printf("\n");

		//Draw dividing line between the rows
		for (column = 0; column < BOARD_WIDTH; column++)
		{
			printf("----");
		}
		printf("\n");
	}

	//free the handle
	CloseHandle(hConsole);

}

int split(const char *str, char c, char ***arr)
{
	int count = 1;
	int token_len = 1;
	int i = 0;
	char *p;
	char *t;

	p = str;
	while (*p != '\0')
	{
		if (*p == c)
			count++;
		p++;
	}

	*arr = (char**)malloc(sizeof(char*) * count);
	if (*arr == NULL)
		exit(1);

	p = str;
	while (*p != '\0')
	{
		if (*p == c)
		{
			(*arr)[i] = (char*)malloc(sizeof(char) * token_len);
			if ((*arr)[i] == NULL)
				exit(1);

			token_len = 0;
			i++;
		}
		p++;
		token_len++;
	}
	(*arr)[i] = (char*)malloc(sizeof(char) * token_len);
	if ((*arr)[i] == NULL)
		exit(1);

	i = 0;
	p = str;
	t = ((*arr)[i]);
	while (*p != '\0')
	{
		if (*p != c && *p != '\0')
		{
			*t = *p;
			t++;
		}
		else
		{
			*t = '\0';
			i++;
			t = ((*arr)[i]);
		}
		p++;
	}

	return count;
}
