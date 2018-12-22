#pragma once
#include "Client.h"

int runClientMode(int argc, char ** argv)
{
	//can assume that number of arguments are given ok from HW
	debugFilePath = argv[2];
	serverPort = argv[3];
	username = argv[4];
	inputMode = argv[5];
	if (!strcmp(INPUT_MODE_FILE, inputMode)) {
		inputFile = argv[6];
	}





	return 0;
}
