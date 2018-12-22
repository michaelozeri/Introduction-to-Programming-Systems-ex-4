#pragma once

#include "globals.h"
#include "Client.h"
#include "Server.h"


int main(int argc, char** argv) {

	//boilerPlate code from recitation must-have to initialize sockets
	WSADATA wsa_data;
	int result;
	// Initialize Winsock
	result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	char* runType = argv[1];
	if (!strcmp(CLIENT_MODE, runType)) {
		return runClientMode(argc, argv);
	}
	else {
		return runServerMode(argc, argv);
	}

	// Deinitialize Winsock - boilerPlate code from recitation must-have to Deinitialize sockets 
	result = WSACleanup();
	if (result != 0) {
		printf("WSACleanup failed: %d\n", result);
		return 1;
	}
}