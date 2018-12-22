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

	//Call the socket function and return its value to the m_socket variable. 
	// For this application, use the Internet address family, streaming sockets, and the TCP/IP protocol.

	// Create a socket.
	mainClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Check for errors to ensure that the socket is a valid socket.
	if (mainClientSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
		return -1;
	}

	//Create a sockaddr_in object clientService and set  values.
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr(LOCALHOST_IP_ADRESS); //Setting the IP address to connect to
	clientService.sin_port = htons(serverPort); //Setting the port to connect to.

	// Call the connect function, passing the created socket and the sockaddr_in structure as parameters. 
	// Check for general errors.
	if (connect(mainClientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		printf("Failed to connect.\n");
		WSACleanup();
		return -1;
	}

	//send and receive data

	if (inputMode == INPUT_MODE_HUMAN) {
		runClientInHumanMode();
	}
	else {
		runClientInFileMode();
	}


	closesocket(mainClientSocket);//TODO: should i close the socket like this?

	return 0;
}

void runClientInHumanMode()
{
	//TODO: finish
}

void runClientInFileMode()
{
	//TODO: finish
}
