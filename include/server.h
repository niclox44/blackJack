#include <server_client.h>
#include <signal.h>

#define MAX_CLIENTS 30
#define MAX_LENTGH_USER 16
#define MAX_LENTGH_PASSWORD 20

typedef struct {
    int socket;
    char name[100];
    int state;
    int points;
    int cards[5][5]; 
    int card_count;
} Player;





void handle_signinit(int sig);
int logIn(char* user, char* password);
//user+pw is correct == 0 / user not correct = -1 / password not correct = 0


int userExists(char* user); //Exists == 0 / Not exists = 1
int checkPassword(char* user, char* password); //Exists == 0 / Not exists = 1
char* getOptionFromMsg(char* message);


/*
int userExists(char* user){
    FILE* users;
    char userCmp[MAX_LENTGH_USER];
    int exists = 0;

    users = fopen("./data/users.txt", "r");

    while(fgets(userCmp,sizeof(userCmp),users) != NULL){

        clearStr(userCmp);

        if(!strcmp(userCmp, user))
        {
            exists = 1;
            break;

        }

        fgets(userCmp,sizeof(userCmp),users); //jump the password's line
            
        

    }

    fclose(users);

    return exists;
};

int checkPassword(char* user, char* password){

    FILE* passwords;
    char passwordCmp[MAX_LENTGH_PASSWORD];
    int exists = 0;
    int state = 0;
    
    passwords = fopen("./data/users.txt", "r");


    if(fgets(passwordCmp,sizeof(passwordCmp),passwords) == NULL){

        return 0;

    } //Jump the user's line


    while(fgets(passwordCmp,sizeof(passwordCmp),passwords) != NULL){
        
        clearStr(passwordCmp);

        if(!strcmp(passwordCmp,password)){

            exists = 1;
            break;

        }

        if(fgets(passwordCmp,sizeof(passwordCmp),passwords) == NULL){

            break;
        } //jump the user's line

        
    }

    return exists;

};

int logIn(char* user, char* password){

    if(!userExists(user)){

        printf("-Err. Usuario <%s> incorrecto.\n",user);
        return 1;

    }else{
        printf("+Ok. Uusario correcto.\n");


    }
};
*/



