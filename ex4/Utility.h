#pragma once

#include "SocketSendRecvTools.h"

int debugToFile(char* log_msg, int msgLen);

int sendMsg(char* msgType, char* params);

int recvMsg(char* inputBuffer);