#include <server.h>

Player clients[MAX_CLIENTS];

int main(int argc, char const *argv[])
{

    int socketDescriptor, newSocketDescriptor, maxSocketDesccriptor, activity;

    struct sockaddr_in sockName, from;

    socklen_t fromLength = sizeof(from);

    char buffer[BUFFER_SIZE];

    fd_set readfds;

    socketDescriptor = socket(AF_INET, SOCK_STREAM,0);
    if(socketDescriptor == -1)
    {
        perror("FAILED SERVER. Open socket error.");
        exit(EXIT_FAILURE);
    }

    sockName.sin_family = AF_INET;
    sockName.sin_port = htons(2060);
    sockName.sin_addr.s_addr = INADDR_ANY;

    if(bind(socketDescriptor, (struct sockaddr *) &sockName, sizeof(sockName)) == -1) {

        perror("FAILED SERVER. Function bind().");
        exit(EXIT_FAILURE);
    }

    if (listen(socketDescriptor, 1) == -1) {
        perror("Error en la operación de listen");
        exit(1);
    }

    

    printf("Servidor escuchando en el puerto %d...\n", htons(sockName.sin_port));

    while(1) {
        
        if((newSocketDescriptor = accept(socketDescriptor, (struct sockaddr*) &from, &fromLength)) == -1){

            perror("FAILED SERVER. Function accept().");
            exit(EXIT_FAILURE);
        }

        printf("Cliente conectado desde %s:%d", inet_ntoa(from.sin_addr), htons(from.sin_port));
    }

    return 0;
}


void setSocketClient();