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
	return 0;
}
