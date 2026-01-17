#include <server_client.h>
#include <signal.h>

int existsUser(const char* username); // 0: does not exist, 1: exists, -1: error
int isUserAuthenticated(const char* username, const char* password); // 0: not authenticated, 1: authenticatedç
int registerUser(const char* username, const char* password); // 0: success, -1: error


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
