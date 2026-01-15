#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define EXIT 0
#define MAX_CLIENTS 30
#define BUFFER_SIZE 250

#define NOT_AUTHENTICATED 0
#define AUTHENTICATED 1
#define WAITING_FOR_GAME 2
#define IN_GAME 3
#define STAND 4


typedef struct {
    int socket;
    char name[100];
    int state;
    int points;
    int cards[5][5]; 
    int card_count;
} Player;

void clearStr(char *buffer) {
    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

char* extractCommand(char* input) {

    char* aux = input;
    char* command = strtok(aux, " ");
    return command;

}