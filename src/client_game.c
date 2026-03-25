#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "client_game.h"
#include "server_client.h"
#include "utils.h"



void gameLoop(int sd)
{
    fd_set readFds, auxFds;
    char buffer[BUFFER_SIZE];

    int status = WAITING_FOR_GAME;

    FD_ZERO(&readFds);
    FD_SET(0, &readFds);     // stdin
    FD_SET(sd, &readFds);    // socket

    printf("\n====================\n");
    printf("|     BLACKJACK     |\n");
    printf("====================\n");
    printf("Comandos:\n");
    printf("  PLAY   -> entrar/crear partida\n");
    printf("  HIT    -> pedir carta\n");
    printf("  STAND  -> plantarse\n");
    printf("  LEAVE  -> salir del juego (vuelve a cliente)\n\n");

    for(;;) {
        auxFds = readFds;

        int rv = select(sd + 1, &auxFds, NULL, NULL, NULL);
        if (rv < 0) {
            perror("select()");
            return;
        }

        // Mensaje del servidor
        if (FD_ISSET(sd, &auxFds)) {

            bzero(buffer, sizeof(buffer));
            int n = (int)recv(sd, buffer, sizeof(buffer) - 1, 0);

            if (n <= 0) {

                if (n == 0) printf("Servidor desconectado.\n");
                else perror("recv()");
                return;

            }
            clearStr(buffer);

            printf("[SERVER] %s\n", buffer);

            if(strcmp(buffer, "START_GAME") == 0){

                printf("[CLIENT]: Entrando al modo de juego solicitado anteriormente.\n");
                continue;

            }
        }

        // Entrada del usuario
        if (FD_ISSET(0, &auxFds)){

            bzero(buffer, sizeof(buffer));

            if (!fgets(buffer, sizeof(buffer), stdin)) return;

            clearStr(buffer);

            if (strcmp(buffer, "LEAVE") == 0) {

                if((send(sd, buffer, sizeof(buffer),0) == -1)){

					perror("CLIENT ERROR. Function send().");
					exit(EXIT_FAILURE);

				}
                printf("Saliendo del modo juego.\n");

                return;

            }else if(strcmp(buffer, "PLAY") == 0 && status != IN_GAME){

                printf("[CLIENT]: Solicitando entrar/crear partida...\n");
                if((send(sd, buffer, sizeof(buffer),0) == -1)){

					perror("CLIENT ERROR. Function send().");
					exit(EXIT_FAILURE);

				}
                status = IN_GAME;

            }
            
        }else if(status == IN_GAME && (FD_ISSET(0, &auxFds))){

            bzero(buffer, sizeof(buffer));

            if (!fgets(buffer, sizeof(buffer), stdin)) return;

            clearStr(buffer);

            if(strcmp(buffer, "HIT") == 0 || strcmp(buffer, "STAND") == 0){

                printf("[CLIENT]: Enviando comando de juego: %s\n", buffer);
                
                if((send(sd, buffer, sizeof(buffer),0) == -1)){
                    perror("CLIENT ERROR. Function send().");
                    exit(EXIT_FAILURE);
                }
            }
        }
    }
}
