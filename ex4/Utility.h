#pragma once

#include "SocketSendRecvTools.h"

/*
this function writes the message given to it into the debug file given in globals
parameters:
log_msg - a pointer to buffer to write into the log file
msgLen - the length of log_msg
return 0 on success else 1
*/
int debugToFile(char* log_msg);

/*
the main function that appends params to the message in the format
given at HW, message_type:params
parameters:
msgType - the message type as defined in HW
params - the parameters
return:
0 on sucess else -1
*/
int sendMsg(char* msgType, char* params);

/*
an overload to the main function that appends params to the message in the format
given at HW, message_type:params
parameters:
msgType - the message type as defined in HW
param1 - the parameter to send as part of the message
param2 - the parameter to send as part of the message
return:
0 on sucess else -1
*/
int sendMsg2Param(char* msgType, char* param1, char* param2);

/*
an overload to the main function that appends params to the message in the format
given at HW, message_type:params
parameters:
msgType - the message type as defined in HW
param1 - the parameter to send as part of the message
param2 - the parameter to send as part of the message
param3 - the parameter to send as part of the message
return:
0 on sucess else -1
*/
int sendMsg3Param(char* msgType, char* param1, char* param2,char* param3);


int recvMsg(char** inputBuffer);

/*
this function receives the user number from the server and returns it. 
if an error occurs the function prints the error and exits
*/
int recvUserNumber(char* inputBuffer);

/***********************************************************
* This function prints the board, and uses O as the holes.
* The disks are presented by red or yellow backgrounds.
* Input: A 2D array representing the board
* Output: Prints the board, no return value
************************************************************/
void PrintBoard(int board[BOARD_HEIGHT][BOARD_WIDTH]);

/*
*this function was taken from https://stackoverflow.com/questions/9210528/split-string-with-delimiters-in-c
* splits the a_str given string by delimiter a_delim and return an array of substrings
*/
int split(const char *str, char c, char ***arr);