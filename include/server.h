#include <server_client.h>
#include <signal.h>

int existsUser(const char* username); // 0: does not exist, 1: exists, -1: error
int isUserAuthenticated(const char* username, const char* password); // 0: not authenticated, 1: authenticatedç
int registerUser(const char* username, const char* password); // 0: success, -1: error
void removePlayer(Player* players, int* nClient, int socket);
void authenticatePlayer(Player* players, int nClient, int socket);
int isFormattedCorrectly(const char* command, const char* username, const char* password);

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

void authenticatePlayer(Player* players, int nClient, int socket){
    
    for(int i=0; i<nClient; i++){

        if(players[i].socket == socket){

            players[i].state = AUTHENTICATED;
            break;

        }
    }
}

int isFormattedCorrectly(const char* command, const char* username, const char* password) {

    if (!username || !password || strcmp(username, "-u") != 0 || strcmp(password, "-p") != 0) {
        return 0; // Incorrect format
    }

    return 1; // Correct format
}

