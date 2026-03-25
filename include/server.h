#include <server_client.h>
#include <signal.h>

void authenticatePlayer(Player* players, int nClient, int socket);
int registerUser(const char* username, const char* password); // 0: success, -1: error
int existsUser(const char* username); // 0: does not exist, 1: exists, -1: error


int isUserAuthenticated(const char* username, const char* password); // 0: not authenticated, 1: authenticatedç
int isFormattedCorrectly(const char* command, const char* username, const char* password);
// int isGameWaiting(GameSession* games, int nClient);
int isPlayerWaitingGame(Player* playersVector);

int setPlayerGameState(Player* players, int nClient, int socket, int state);

int initializeGameSession(GameSession sessions, Player player);
int dealInitialCards(GameSession* session, int id, int nSession);

void removePlayer(Player* players, int* nClient, int socket);


int registerUser(const char* username, const char* password)
{
    FILE* file = fopen("/home/nicolas/Escritorio/blackjack/data/users.txt", "a");

    if (file == NULL) {
        perror("Error opening users file");
        return -1;

    }

    fprintf(file, "%s %s\n", username, password);
    fclose(file);
    return 0; // Success

}

int existsUser(const char* username)
{
    FILE* file = fopen("/home/nicolas/Escritorio/blackjack/data/users.txt", "r");

    if (file == NULL) {
        perror("Error opening users file");
        return -1;

    }

    char line[256];

    while(fgets(line, sizeof(line), file)){

        char file_username[100];
        sscanf(line, "%s", file_username);

        if(strcmp(username, file_username) == 0){

            fclose(file);
            return 1; // User exists

        }

    }

    fclose(file);
    return 0; // User does not exist

}



int isUserAuthenticated(const char* username, const char* password)
{
    FILE* file = fopen("data/users.txt", "r");

    if (file == NULL) {
        perror("Error opening users file");
        return -1;

    }

    char line[256];

    while(fgets(line, sizeof(line), file)){

        char file_username[100];
        char file_password[100];

        sscanf(line, "%s %s", file_username, file_password);

        if(strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0){

            fclose(file);
            return 1; // Authenticated

        }

    }

    return 0; // Not authenticated

}

 
void removePlayer(Player* players, int* nClient, int socket){

    for(int i=0; i<*nClient; i++){

        if(players[i].socket == socket){
            // Shift player to the left

            for(int j=i; j<*nClient; j++){
                players[j] = players[j+1];
            }

            (*nClient)--; // Decrease the number of clients
            break;
        }

    }
}

// void clearPlayers(Player* players, int nClient){ 

//     for(int i=0; i<nClient; i++){

       
//         free(&players[i].socket);
//         free(&players[i].points);
//         free(&players[i].state);
//         free(&players[i].cards);
//         free(&players[i].card_count);
//         free(&players[i]);

//     }

// }


void authenticatePlayer(Player* players, int nClient, int socket){
    
    for(int i=0; i<nClient; i++){

        if(players[i].socket == socket){

            players[i].state = AUTHENTICATED;
            break;

        }
    }
}

//Check format of INICIO and REGISTRO commands
int isFormattedCorrectly(const char* command, const char* username, const char* password) {

    if (!username || !password || strcmp(username, "-u") != 0 || strcmp(password, "-p") != 0) {
        return 0; // Incorrect format
    }

    return 1; // Correct format
}

//Funcion inutilizada, ya que ya se comprueba en setPlayerGameSession(...);

// int isGameWaiting(GameSession* game, int nClient){

//     int n = nClient/2;

//     for(int i=0; i<n; i++){

//         if(game[i].status == 0){ //Esperando a un jugador

//             return 1;
//             break;  
//         }

//     }
//     return 0; //Ninguna sesion en estado de waiting
// }

int initializeGameSession(GameSession sessions, Player player){


        sessions.id = player.socket;
        sessions._player[0] = &player;
        sessions._player[1] = NULL; // Esperando a otro jugador

        // Initialize deck
        for(int j=0; j<=51; j++){
            sessions.deck[j] = (j % 13) + 1  ; // suit, rank, dealt (0 = not dealt, 1 = dealt)
        }

        // Shuffle deck
        srand((unsigned int)time(NULL));
        for(int j=0; j<52; j++){
            int r = rand() % 52;
            int temp = sessions.deck[j];
            sessions.deck[j] = sessions.deck[r];
            sessions.deck[r] = temp;
        }



    return 1;
}


int setPlayerGameState(Player* players, int nClient, int socket, int state){

    for(int i=0; i<nClient; i++){

        if(players[i].socket == socket){

            players[i].state = state;
            
            return 1;

        }
    }
    return 0;
}

int isPlayerWaitingGame(Player* playersVector){

    int n = sizeof(playersVector)/sizeof(Player*);

    for(int i=0; i<n; i++){

        if(playersVector[i].state == WAITING_FOR_GAME){ //Jugador esperando

            return i; //Devolvemos el indice del jugador que esta esperando

        }
        
    }

    return -1; //Ningun jugador esperando
}

int findCLientBySocket(Player* playersVector, int nClient, int socket){

    for(int i=0; i<nClient; i++){

        if(playersVector[i].socket == socket){

            return i;
        }
    }

    return -1; //No se ha encontrado el cliente
}






