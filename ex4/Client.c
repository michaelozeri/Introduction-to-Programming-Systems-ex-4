#pragma once
#include "Client.h"
#include "Utility.h"

int runClientMode(int argc, char ** argv)
{
	//can assume that number of arguments are given ok from HW
	debugFilePath = argv[2];
	serverPort = atoi(argv[3]);
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
	InetPton(AF_INET, LOCALHOST_IP_ADRESS, &clientService.sin_addr.s_addr); //Setting the IP address to connect to - TODO:check!
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
	debugToFile("Sending username to Server\n");
	if (!sendMsg(NEW_USER_REQUEST, username)) {
		debugToFile("ERROR: sending username to server\n");
	}
	//server - NEW_USER_ACCEPTED and sent client number
	MyUserNumber = recvUserNumber(userBuffer);
	debugToFile("user number is:");
	debugToFile(MyUserNumber);
	//server - GAME_STARTED
	receiveAndVerifyGameStarted();
	//server - BOARD_VIEW
	recvBoardAndPrint();
	bool shouldExit = false;
	while (!shouldExit) {
		//server - TURN_SWITCH
		if (recvTurnSwithAndCheckisMyTurn()) {
			printf("Please insert your play:\n");
			//user - PLAY_REQUEST
			int columnToInsert;
			scanf_s("%d", &columnToInsert);
			char playBuf[10];
			sendMsg(PLAY_REQUEST, _itoa_s(columnToInsert, playBuf, 10,10));
			//server - PLAY_ACCEPTED
			recvPlayAccepted(userBuffer);
		}
		//server - BOARD_VIEW
		recvBoardAndPrint();
	}
}

void receiveAndVerifyGameStarted()
{
	recvMsg(&userBuffer);
	if (strcmp(userBuffer, GAME_STARTED)) {
		debugToFile("ERROR: should be game started\n");
	}
}

void runClientInFileMode()
{
	//TODO: finish
}

int recvBoardAndPrint()
{
	char** messageAndParams;
	char** params;
	int board[BOARD_HEIGHT][BOARD_WIDTH];
	recvMsg(&userBuffer);
	split(userBuffer, ':', &messageAndParams);//split messege and params
	if (strcmp(messageAndParams[0], BOARD_VIEW)) {
		debugToFile("ERROR: should receive board view\n");
		return -1;
	}
	split(messageAndParams[1], ';', &params);
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		for (int j = 0; j < BOARD_WIDTH; j++) {
			board[i][j] = atoi(params[(i*BOARD_WIDTH) + j]);
		}
	}
	PrintBoard(board);
	return 0;
}

bool recvTurnSwithAndCheckisMyTurn()
{
	char** messageAndParams;
	recvMsg(&userBuffer);
	split(userBuffer, ':', &messageAndParams);//split messege and params
	if (strcmp(messageAndParams[0], TURN_SWITCH)) {
		if (strcmp(messageAndParams[0], GAME_ENDED)) {
			debugToFile("ERROR: should receive turn switch or game ended\n");
			return -1;
		}
		else {
			debugToFile("GAME_ENDED received from server - logging out");
		}
	}
	return !strcmp(username, messageAndParams[1]);
}
