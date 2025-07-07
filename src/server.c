#include <server.h>

Player clients[MAX_CLIENTS];

int main(int argc, char const *argv[])
{

    int socketDescriptor, newSocketDescriptor, maxSocketDesccriptor, activity;
    struct sockaddr_in sockName, from;
    char buffer[BUFFER_SIZE];
    fd_set readfds;
    socklen_t fromLength = sizeof(from);

    socketDescriptor = socket(AF_INET, SOCK_STREAM,0);

    if(socketDescriptor == 1)
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

    if (listen(socketDescriptor, 3) == -1) {
        perror("Error en la operación de listen");
        exit(1);
    }

    printf("Servidor escuchando en el puerto 2060...\n");

    while(1){
        // Limpiar el conjunto de descriptores
        FD_ZERO(&readfds);

        // Añadir el socket maestro al conjunto
        FD_SET(socketDescriptor, &readfds);
        maxSocketDesccriptor = socketDescriptor;
    }

    return 0;
}


void setSocketClient();