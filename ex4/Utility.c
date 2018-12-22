#pragma once

#include "Utility.h"

int debugToFile(char * log_msg, int msgLen)
{
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


void PrintBoard(int board[][BOARD_WIDTH])
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
