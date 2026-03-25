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
    int state; // NOT_AUTHENTICATED, AUTHENTICATED, WAITING_FOR_GAME, IN_GAME
    int points;
    int cards[5][5]; // [][0]=suit, [][1]=rank
    int card_count;
} Player;

typedef struct {
    int id;
    Player* _player[2]; // Puntero a los jugadores que participan en la partida
    int deck[51];
} GameSession;

enum suits {
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};


