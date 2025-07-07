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

void setClientSocket();
int findUser(char* name);
int checkPassword(char* name, char* password);
int extractSubstring(char* string, char* name, char* password);
void registration(char* name, char* passwd);
void startGame(Player *player1, Player *player2);
void drawCard(Player *player);
void checkGameStatus(Player *player1, Player *player2);
int findWaitingPlayer(int current);
void sendMessage(int sockfd, const char* format, ...);