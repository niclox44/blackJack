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
#include <states.h>
#include <signal.h>


typedef struct {
    int socket;
    char name[100];
    int state;
    int points;
    int cards[5][5]; 
    int card_count;
} Player;

void setClientSocket();
void handle_signinit(int sig);
int findUser(char* name);
int checkPassword(char* name, char* password);
int extractSubstring(char* string, char* name, char* password);
void registration(char* name, char* passwd);
void startGame(Player *player1, Player *player2);
void drawCard(Player *player);
void checkGameStatus(Player *player1, Player *player2);
int findWaitingPlayer(int current);
void sendMessage(int sockfd, const char* format, ...);
