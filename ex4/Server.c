#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "sys/types.h"
#include "in.h"
#include "pthread.h"
#include "globals.h"
#include "ServerUtility.h"
#include "SocketSendRecvTools.h"

#define MESSAGE_BUFFER 500
#define CLIENT_ADDRESS_LENGTH 100


Mutex * Turn;
Mutex* Message;
// Start server
void * start_server(int socket_fd, struct sockaddr_in *address) {
    bind(socket_fd, (struct sockaddr *) address, sizeof(struct sockaddr_in*));
    printf("Waiting for connection...\n");
    listen(socket_fd, 10);
}

char** str_split(char* a_str, const char a_delim)
{
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
    knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

char** names;
int numofclients = 0;


// Get message from stdin and send to client
void * send_message(int new_socket_fd, struct sockaddr *cl_addr, char* message) {
    sendto(new_socket_fd, message, MESSAGE_BUFFER, 0, (struct sockaddr *) &cl_addr, sizeof cl_addr);
}

void * clientHandler(void * socket, void* clientNumber, void * name, void * oName) {
    SOCKET socket_fd;
    int response;
    char message[MESSAGE_BUFFER];
    char tempmessage[100];
    char str[MESSAGE_BUFFER];
    memset(message, 0, MESSAGE_BUFFER); // Clear message buffer
    memset(str, 0, MESSAGE_BUFFER); // Clear message buffer
    socket_fd = (SOCKET)socket;
    int clientN = (int)clientNumber;
    char* myName = (char*)name;
    char* playerName = (char*)oName;
    int myColor = 0;
    int order = 0;
    int sendBoardView = 0;
    int sendTurn = 0;
    int newChatMsgArrived = 0;
    memset(str, 0, MESSAGE_BUFFER); // Clear message buffer
    BoardView* board = InitializeBoardView();
    if (strcmp(myName, names[0]))
    {
        myColor = RED;
    }
    else
    {
        myColor = YELLOW;
    }

    // Define name
    while (true)
    {
        if (!strcmp(tempmessage, Message->turn))
        {
            SendMsg(RECEIVE_MESSAGE, Message->turn, socket_fd);
            strncpy(tempmessage, Message->turn, strlen(Message->turn));
            memset(newChatMsgArrived + strlen(Message->turn), '\0', 100 - strlen(Message->turn));
        }

        if (numofclients == 2 && order == 0)
        {
            if (SendMsg(GAME_STARTED, NULL, socket_fd) == -1) exit(-1);
            char * boardView = ConvertBoardView(board);
            if (SendMsg(BOARD_VIEW, boardView, socket_fd) == -1) exit(-1);
            if (SendMsg(TURN_SWITCH, Turn->turn, socket_fd) == -1) exit(-1);
            order++;
        }

        if (order == 2)
        {
        }

        response = recvfrom(socket_fd, message, MESSAGE_BUFFER, 0, NULL, NULL);
        if (response)
        {
            char** messageS = str_split(message, ':');
            if (messageS == NULL)
            {
                print("error");
                return -1;
            }

            if (strcmp(messageS[0], SEND_MESSAGE))
            {
                if (WaitForSingleObject(Message->handle, INFINITE) != WAIT_OBJECT_0) {
                    error("couldnt obtain mutex");
                    return -1;
                }
                if (!strcmp(tempmessage, Message->turn))
                {
                    SendMsg(RECEIVE_MESSAGE, Message->turn, socket_fd);
                }
                
                strncpy(Message->turn, messageS[1], strlen(messageS[1]));
                strncpy(tempmessage, Message->turn, strlen(Message->turn));
                memset(tempmessage + strlen(messageS[1]), '\0', 100 - strlen(messageS[1]));
                memset(Message->turn + strlen(messageS[1]), '\0', 100 - strlen(messageS[1]));
                ReleaseMutex(Message->handle);
            }

            if (!strcmp(myName, Turn->turn))
            {
                SendMsg(PLAY_DECLINED, NOT_YOUR_TURN, socket_fd);
                continue;
            }

            if (strcmp(messageS[0], PLAY_REQUEST))
            {
                if (InsertToBoard(board, messageS[1], myColor))
                {
                    int result = GetBoardWinner(board);
                    if (result == -1)
                    {
                        print("error");
                        return -1;
                    }

                    char* boardView = ConvertBoardView(board);
                    SendMsg(PLAY_ACCEPTED, NULL, socket_fd);
                    SendMsg(BOARD_VIEW, boardView, socket_fd);
                    if (result == 0)
                    {
                        SendMsg(TURN_SWITCH, Turn->turn, socket_fd);
                        Turn->turn = playerName;
                        continue;
                    }
                    else if (result == 1)
                    {
                        SendMsg(GAME_ENDED, "Tie", socket_fd);
                    }
                    else if (result == RED)
                    {
                        SendMsg(GAME_ENDED, names[0], socket_fd);
                    }
                    else if (result == YELLOW)
                    {
                        SendMsg(GAME_ENDED, names[1], socket_fd);
                    }
                }
                else
                {
                    SendMsg(PLAY_DECLINED, IILEGAL_MOVE, socket_fd);
                    continue;
                }
            }
        }
    }
}

int runServerMode(int argc, char**argv) {
    if (argc < 4) {
        printf("wrong number of arguments");
        exit(-1);
    }

    long port = strtol(argv[3], NULL, 10);
    char* logFile = argv[2];
    struct sockaddr_in* address = CreateNewStructSocket(port);
    int length, response, new_socket_fd;
    int numbOfConnections = 0;
    int gotRequest = 0;
    char message[MESSAGE_BUFFER];
    memset(message, 0, MESSAGE_BUFFER);
    char client_address[CLIENT_ADDRESS_LENGTH];
    pthread_t threads[2];

    SOCKET socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    start_server(socket_fd, &address);

    // Accept connection
    while (true)
    {
        struct sockaddr_in *cl_addr = (struct sockaddr_in*) malloc(sizeof(struct sockaddr_in));
        length = sizeof(cl_addr);
        new_socket_fd = accept(socket_fd, (struct sockaddr *) cl_addr, &length);
        if (new_socket_fd < 0) {
            printf("Failed to connect\n");
            exit(-1);
        }

        if (numofclients >= 2)
        {
            SendMsg(NEW_USER_DECLINED, NULL, new_socket_fd);
            continue;
        }

        while (true)
        {
            response = recvfrom(new_socket_fd, message, MESSAGE_BUFFER, 0, NULL, NULL);
            if (response)
            {
                char** messageS = str_split(message, ':');
                if (messageS == NULL)
                {
                    break;
                }
                if (!strcmp(messageS[0], NEW_UESR_REQUEST))
                {
                    SendMsg(NEW_USER_DECLINED, NULL, new_socket_fd);
                    break;
                }

                names[numbOfConnections] = (char*)malloc(sizeof(char)*strlen(messageS[1]));
                if (names == NULL)
                {
                    printf("allocating failed");
                    return -1;
                }

                strncpy(names[numbOfConnections], messageS[1], strlen(messageS[1]));
                if (numbOfConnections == 0)
                {
                    Turn = CreateClientsMutex(names[numbOfConnections]);
                    char* msgBuf = (char*)malloc(sizeof(char) * 100);
                    Message = CreateClientsMutex(msgBuf);
                }

                gotRequest = 1;
                break;
            }
        }

        if (gotRequest == 1)
        {
            gotRequest = 0;
            inet_ntop(AF_INET, &(cl_addr->sin_addr), client_address, CLIENT_ADDRESS_LENGTH);
            printf("Connected: %s\n", client_address);
            // Create new thread to receive messages
            pthread_create(&threads[numbOfConnections], NULL, clientHandler, (void *)new_socket_fd, numbOfConnections);
            numbOfConnections++;
        }
    }

    // Close sockets and kill thread
    close(new_socket_fd);
    close(socket_fd);
    pthread_exit(NULL);
    return 0;
}