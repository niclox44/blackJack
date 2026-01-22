#include <server.h>

int masterSocket;


int main(int argc, char const *argv[])
{
    struct sockaddr_in serverAddr, clientAddr;

    socklen_t addrLength;

    char buffer[BUFFER_SIZE];

    fd_set setfds, master;
    //setfds: copia temporal para pasar a select
    //master: conjunto de todos los sockets activos

    Player playersVector[MAX_CLIENTS];

    int newSocketDescriptor, maxFds, nClient = 0;

    masterSocket = socket(AF_INET, SOCK_STREAM,0);
    if(masterSocket == -1)
    {
        perror("FAILED SERVER. Open socket error.");
        exit(EXIT_FAILURE);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2060);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    //Give the socket FD the local address ADDR (which is LEN bytes long).

    int bindValue = bind(masterSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if(bindValue == -1) {

        perror("FAILED SERVER. Function bind()");
        exit(EXIT_FAILURE);
    }

    int listenValue = listen(masterSocket, 1);
    if (listenValue == -1) {
        perror("Error en la operación de listen");
        exit(1);
    }

    

    printf("Servidor escuchando en el puerto %d...\n", htons(serverAddr.sin_port));

    //Inicializamos el conjutno de sockets y añadimos el maestro
    FD_ZERO(&master);
    FD_SET(masterSocket, &master);

    maxFds = masterSocket;
    
    while(1) {
        
        setfds = master; //Copia de master ya que select lo modifica

        //select: Vigilamos si ha llegado informacion al nodo maestro (Nueva conexion).
        int selectValue = select(maxFds + 1, &setfds, NULL, NULL, NULL); 

        if(selectValue < 0){

            perror("-Err. Server error. Select function.\n");
            break;

        }

        //Master recibe una nueva conexion
        if(FD_ISSET(masterSocket, &setfds)){
            
            addrLength = sizeof(clientAddr);
            newSocketDescriptor = accept(masterSocket, (struct sockaddr* )&clientAddr, &addrLength);
            
            if(newSocketDescriptor < 0){

                perror("-Err. Server error. Select() function.\n");
                break;

            }else{

                printf("Cliente conectado desde %s:%d\n", inet_ntoa(clientAddr.sin_addr), htons(clientAddr.sin_port));
                FD_SET(newSocketDescriptor, &master);

                playersVector[nClient].socket = newSocketDescriptor;
                playersVector[nClient].state = NOT_AUTHENTICATED;
                playersVector[nClient].points = 0;
                playersVector[nClient].card_count = 0;
                playersVector[nClient].name[0] = '\0';

                nClient++;

                if(newSocketDescriptor > maxFds){
                    
                    maxFds = newSocketDescriptor;

                }
            }
        }

        //Recorremos el resto de socket para ver si tienen datos.
        for(int sd=0; sd<= maxFds; sd++){

            if(sd == masterSocket) continue; 

            if(FD_ISSET(sd, &setfds)){

                int recive = recv(sd, buffer, sizeof(buffer), 0);

                if( recive <= 0){

                    if(recive == 0) {

                        printf("El cliente %d se ha desconectado\n", sd);
                        removePlayer(playersVector, &nClient, sd);

                    }
                    else perror("-Err. Server error. Recv() function.\n");

                    close(sd);
                    FD_CLR(sd, &master);

                }else{

    
                    printf("+Cliente %d: %s\n", sd, buffer);
                    
                    char* util = NULL;
                    char* command = strtok_r(buffer, " ", &util);

                    if(strcmp(command, "SALIR") == 0){

                        printf("El cliente %d ha solicitado salir.\n", sd);
                        removePlayer(playersVector, &nClient, sd);
                        close(sd);
                        FD_CLR(sd, &master);
                        
                    }else if(strcmp(command, "INICIO") == 0){

                        char *u = strtok_r(NULL, " ", &util);
                        char *user = strtok_r(NULL, " ", &util);
                        char *p = strtok_r(NULL, " ", &util);
                        char *password = strtok_r(NULL, " ", &util);

                        if(isFormattedCorrectly(command, u, p) == 0) { //Check format
                            char response[] = "ERROR. Formato incorrecto. Use: INICIO -u <usuario> -p <contraseña>\n";
                            send(sd, response, sizeof(response), 0);
                            continue;
                        }

                        if(isUserAuthenticated(user, password) == 1){

                            
                            char response[] = "Usuario autenticado correctamente.\n";
                            for(int i=0; i<nClient; i++){

                                if(playersVector[i].socket == sd){

                                    strncpy(playersVector[i].name, user, sizeof(playersVector[i].name)-1);
                                    playersVector[i].name[sizeof(playersVector[i].name)-1] = '\0';
                                    break;

                                }
                            }       
                            send(sd, response, sizeof(response), 0);

                        }else{

                            char response[] = "ERROR. Usuario o contraseña incorrectos.\n";
                            send(sd, response, sizeof(response), 0);

                        }

                    

                    }else if (strcmp(command, "REGISTRO") == 0) {

                        char *u = strtok_r(NULL, " ", &util);
                        char *user = strtok_r(NULL, " ", &util);
                        char *p = strtok_r(NULL, " ", &util);
                        char *password = strtok_r(NULL, " ", &util);

                        if(isFormattedCorrectly(command, u, p) == 0) { //Check format
                            char response[] = "ERROR. Formato incorrecto. Use: Registro -u <usuario> -p <contraseña>\n";
                            send(sd, response, sizeof(response), 0);
                            continue;
                        }

                        if (existsUser(user) == 0) {
                            if (registerUser(user, password) == 0) {

                                char response[] = "Usuario registrado correctamente.\n";
                                send(sd, response, sizeof(response), 0);

                            } else {

                                char response[] = "ERROR. No se pudo registrar el usuario.\n";
                                send(sd, response, sizeof(response), 0);

                            }
                        } else {

                            char response[] = "ERROR. El usuario ya existe.\n";
                            send(sd, response, sizeof(response), 0);

                        }
                    }else if(strcmp(command, "PLAY") == 0){

                        char response[] = "Modo juego no implementado aún.\n";
                        send(sd, response, sizeof(response), 0);

                    }


                    /* 
                    ---->ESTO ES SOLO PARA PROBAR QUE RECIBE EL SERVIDOR<----

                    char confirmation[] = "Mensaje recibido\n";
                    char confirmation[] = "Mensaje recibido\n";
                    send(sd, confirmation, sizeof(confirmation), 0);
                    */
                }
            }

        }

            
    }
    close(masterSocket);

    return 0;
}

