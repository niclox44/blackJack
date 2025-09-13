#include <server.h>

Player clients[MAX_CLIENTS];

int socketDescriptor;

void handle_siginit(int sig){

    printf("\n[!] Capturado Ctrl+C, cerrando socket...\n");
    close(socketDescriptor);   // cerramos el socket
    exit(0);

}

int main(int argc, char const *argv[])
{

    int newSocketDescriptor, maxSocketDesccriptor, activity;

    struct sockaddr_in sockName, from;

    socklen_t fromLength = sizeof(from);

    char buffer[BUFFER_SIZE];

    fd_set readfds;

    int status = !EXIT;

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

        perror("FAILED SERVER. Function bind()");
        exit(EXIT_FAILURE);
    }

    if (listen(socketDescriptor, 1) == -1) {
        perror("Error en la operación de listen");
        exit(1);
    }

    

    printf("Servidor escuchando en el puerto %d...\n", htons(sockName.sin_port));

    signal(SIGINT,handle_siginit);
    while(1) {
        
        if((newSocketDescriptor = accept(socketDescriptor, (struct sockaddr*)&from, &fromLength)) == -1){

            perror("FAILED SERVER. Function accept().");
            exit(EXIT_FAILURE);
        }

        printf("Cliente conectado desde %s:%d\n", inet_ntoa(from.sin_addr), htons(from.sin_port));

        do{
                /*GOING to RECIVE the message from the CLIENT*/
            bzero(buffer, sizeof(buffer));
            if(recv(newSocketDescriptor,buffer,sizeof(buffer),0) == -1){
                perror("SERVER ERROR. Function recv().");
                exit(EXIT_FAILURE);
            }

            printf("Mensaje del cliente: %s\n",buffer);

            if(!strcmp(buffer,"SALIR")){
                 printf("Cliente conectado desde %s:%d se ha desconectado.\n", inet_ntoa(from.sin_addr), htons(from.sin_port));
                status = EXIT;
            }

            /*GOING to SEND the message from the CLIENT*/

            bzero(buffer,sizeof(buffer));
            strcpy(buffer,"+Ok. Mensaje recibido.");


            if(send(newSocketDescriptor,buffer, sizeof(buffer),0) == -1){
                perror("SERVER ERROR. Function send()\n");
                exit(EXIT_FAILURE);
            }
        
        }while( status );

            close(newSocketDescriptor);
        }
        close(socketDescriptor);

    return 0;
}


void setSocketClient();