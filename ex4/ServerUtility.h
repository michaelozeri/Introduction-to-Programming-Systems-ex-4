#pragma once
#include "globals.h"

#define SEMAPHORE_INITIAL_VALUE 1
#define SEMAPHORE_MAX_VALUE 1

/*
struct representing the semaphore has a handle to the semaphore
*/
typedef struct Semaphore {
    HANDLE handle;
}Semaphore;

/* ThreadParams is a struct representing the params given to a command thread
*	Members:
*			Command- the command to run by the process of the thread
*			ResultsPath - the path to the output file created by the process
*			ExpectedResultPath- the path to the expected results file to compare with the actual output file created by the processs
*			ReturnCode - the code returned by the thread.
*/
typedef struct ThreadParams
{
    SOCKET socket;
    int clientNumber;
    char* name;
    char* playerName;
} ThreadParams;


/* Thread is a struct representing a new thread
*	Members:
*			Id - thread id
*			Handle - thread handle
*			WaitCode - the code we got from waiting on the thread
*			ExitCode- the exit code of the thread
*			ReturnValue - the value returned by the last function done on the thread
*			p_thread_params - the arguments for the thread
*/
typedef struct Thread {
    HANDLE Handle;
    DWORD* Id;
    DWORD WaitCode;
    LPTHREAD_START_ROUTINE Function;
    ThreadParams *threadParams;
} Thread;
    

Thread * defineNewThread(SOCKET socket, int clientNumber, char* name);

/*
struct representing the mutex has a handle to the mutex
*/
typedef struct Mutex {
    HANDLE handle;
    char* turn;
}Mutex;

/*
Creates new struct for server socket
*/
struct sockaddr_in *CreateNewStructSocket(long port);

/*
struct representing the BoardView
*/
typedef struct BoardView {
    int** boardPlacements;
}BoardView;

/*
this function creates the semaphore for two clients
returns:
Semaphore* if sucess else NULL
*/
Semaphore* CreateClientsSemaphore();



/*
this function inserts new play to board
parameters:
board -> the board to insert play to
rowC -> the row to insert into
color -> the color of player -> RED or YELLOW
returns:
0 if failed- bad play
1 if succeeded
*/
int InsertToBoard(BoardView* board, char* rowC, int color);


/*
this function creates a mutex
returns:
a pointer to a mutex if success else NULL
*/
Mutex * CreateClientsMutex(char* name);

/*
the main function that appends params to the message in the format
given at HW, message_type:param
parameters:
msgType - the message type as defined in HW
param - The param
socket - The socket
return:
0 on sucess else -1
*/
int SendMsg(char* msgType, char* param, SOCKET socket);

/*
Transforms  board view to string
paramters:
board - the 2d matrix representing board
return:
the transformed boardview or null if errors
*/
char * ConvertBoardView(BoardView* board);

/*
Initializes board's view
return:
initalized boardview or null if errors
*/
BoardView* InitializeBoardView();

/*
Check board for winner
Parameters:
board - the board to check
return:
RED if red wins
YELLOW if yellow wins
1 if tie
0 if no result yet and board is not full
-1 if errors
*/
int GetBoardWinner(BoardView* board);
