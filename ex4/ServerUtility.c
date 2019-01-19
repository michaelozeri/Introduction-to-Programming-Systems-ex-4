#include "ServerUtility.h"
#include "SocketSendRecvTools.h"

Semaphore * CreateClientsSemaphore()
{
    Semaphore * sem = (Semaphore*)malloc(sizeof(Semaphore));
    if (sem == NULL) {
        return NULL;
    }
    //create semaphore with size of outputbuffer
    sem->handle = CreateSemaphore(
        NULL,
        SEMAPHORE_INITIAL_VALUE,
        2, "bufferQueueSemaphore");
    if (sem->handle == NULL) {
        return NULL;
    }

    return sem;
}

Thread * defineNewThread(SOCKET socket, int clientNumber, char* name, char* playerName)
{
    Thread* newThread = (Thread*)malloc(sizeof(Thread));
    if (newThread == NULL) {
        return NULL;
    }

    newThread->threadParams = (ThreadParams*)malloc(sizeof(ThreadParams));
    if (newThread->threadParams == NULL) {
        return NULL;
    }

    //set parameters of thread
    newThread->threadParams->socket = socket;
    newThread->threadParams->clientNumber = clientNumber;
    newThread->threadParams->name = name;

    newThread->Id = (DWORD*)malloc(sizeof(DWORD));
    if (newThread->Id == NULL) {
        return NULL;
    }

    return newThread;
}

Mutex * CreateClientsMutex(char* name)
{
    Mutex* mutex = (Mutex*)malloc(sizeof(Mutex));
    if (mutex == NULL) {
        return NULL;
    }

    mutex->handle = CreateMutex(
            NULL, // can be set to null from recitation
            false, // thread calling create Mutex should be its initial owner?
            NULL); // its possible to use null but we need to make sure not to lose handle
    if (mutex->handle == NULL) {
        return NULL;
    }
    
    mutex->turn = name;
    return mutex;
}

BoardView* InitializeBoardView()
{
    BoardView* board = (BoardView*)malloc(sizeof(BoardView));
    if (board == NULL)
    {
        return NULL;
    }

    board->boardPlacements = (int**)malloc(sizeof(int) * 6);
    for (int i = 0; i < 6; i++)
    {
        board->boardPlacements = (int*)malloc(sizeof(int) * 7);
        for (int j = 0; j < 7; j++)
        {
            board->boardPlacements[i][j] = 0;
        }
    }

    return board;
}

int GetPlacementLine(BoardView* board, int row)
{
    for (int i = 0; i < 6; i++)
    {
        if (board->boardPlacements[row][i] == 0)
        {
            return i;
        }
    }
    
    return -1;
}

int GetRowNumber(char* row)
{
    if (strcmp(row, '1')) return 0;
    if (strcmp(row, '2')) return 1;
    if (strcmp(row, '3')) return 2;
    if (strcmp(row, '4')) return 3;
    if (strcmp(row, '5')) return 4;
    if (strcmp(row, '6')) return 5;
    if (strcmp(row, '7')) return 6;
}

int InsertToBoard(BoardView* board, char* rowC, int color)
{
    int row = GetRowNumber(rowC);
    int line = GetPlacementLine(board, row);
    if (line == -1)
    {
        return 0;
    }

    board->boardPlacements[row][line] = color;
    return 1;
}

struct sockaddr_in *CreateNewStructSocket(long port)
{
    struct sockaddr_in *address = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = port;
    return address;
}

char* ConvertBoardView(BoardView* board)
{
    int lastWritten = 0;
    int sumWritten = 0;
    char* boardView[SEND_BUFFER_SIZE];
    memset(boardView, 0, SEND_BUFFER_SIZE);
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            lastWritten = sprintf(boardView + sumWritten, "%d ", board->boardPlacements[i][j]);
            sumWritten += lastWritten;
        }

        boardView[sumWritten] = ';';
        sumWritten += 1;
    }
}

int GetBoardWinner(BoardView* board)
{
    int isBoardFull = 1;
    if (board == NULL) return -1;
    int** displacements = board->boardPlacements;
    if (displacements == NULL) return -1;
    for (int i = 0; i < BOARD_HEIGHT; i++)
    {
        for (int j = 0; j < BOARD_WIDTH; j++)
        {
            if (displacements[i][j] == 0)
            {
                isBoardFull = 0;
            }

            if (BOARD_HEIGHT - i >= 4)
            {
                if ((displacements[i][j] != 0) &&
                    (displacements[i][j] == displacements[i + 1][j]) &&
                    (displacements[i][j] == displacements[i + 2][j]) &&
                    (displacements[i][j] == displacements[i + 3][j]))
                {
                    return displacements[i][j];
                }
            }

            if (BOARD_WIDTH - j >= 4)
            {
                if ((displacements[i][j] != 0) &&
                    (displacements[i][j] == displacements[i][j + 1]) &&
                    (displacements[i][j] == displacements[i][j + 2]) &&
                    (displacements[i][j] == displacements[i][j + 3]))
                {
                    return displacements[i][j];
                }
            }

            if ((BOARD_WIDTH - j >= 4) && (BOARD_HEIGHT - i >= 4))
            {
                if ((displacements[i][j] != 0) &&
                    (displacements[i][j] == displacements[i + 1][j + 1]) &&
                    (displacements[i][j] == displacements[i + 2][j + 2]) &&
                    (displacements[i][j] == displacements[i + 3][j + 3]))
                {
                    return displacements[i][j];
                }
            }
        }
    }

    return isBoardFull;
}

int SendMsg(char * msgType, char * param, SOCKET socket)
{
    int retVal;
    TransferResult_t transferResult;
    char buffer[SEND_BUFFER_SIZE];
    if (param != NULL)
    {
        retVal = sprintf_s(buffer, SEND_BUFFER_SIZE, "%s:%s", msgType, param);
    }
    else
    {
        retVal = strncpy(buffer, msgType, strlen(msgType));
    }

    if (retVal < 0) {
        return -1;
    }
    transferResult = SendBuffer(buffer, retVal, socket);
    if (transferResult != TRNS_SUCCEEDED) {
        return -1;
    }
    return 0;
}