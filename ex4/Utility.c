#pragma once

#include "Utility.h"

int debugToFile(char * log_msg, int msgLen)
{
	int retval;
	if (debugFile == NULL) {
		debugFile = fopen(debugFilePath, "a"); //open with append to write to end of file
		if (debugFile == NULL)
		{
			//TODO: finish here
			printf("Error opening file!\n");
			exit(1);
		}
	}
	retval = fprintf(debugFile, "%s\n", log_msg);
	if (retval != msgLen) {
		//TODO: finish here
		printf("Error opening file!\n");
		exit(1);
	}
	//fclose(debugFile); close only on end of main
	return 0;
}
