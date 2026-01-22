#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>

#include "client_game.h"
#include "server_client.h"
#include "utils.h"

static int send_line(int sd, const char *line) {
    size_t n = strlen(line);
    // Enviar solo lo necesario (NO sizeof(buffer))
    return (int)send(sd, line, n, 0);
}

void gameLoop(int sd)
{
    fd_set readFds, auxFds;
    char buffer[BUFFER_SIZE];

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

            memset(buffer, 0, sizeof(buffer));
            int n = (int)recv(sd, buffer, sizeof(buffer) - 1, 0);

            if (n <= 0) {

                if (n == 0) printf("Servidor desconectado.\n");
                else perror("recv()");
                return;

            }
            clearStr(buffer);

            printf("[SERVER] %s\n", buffer);
        }

        // Entrada del usuario
        if (FD_ISSET(0, &auxFds)) {

            bzero(buffer, sizeof(buffer));

            if (!fgets(buffer, sizeof(buffer), stdin)) return;

            clearStr(buffer);

            if (strcmp(buffer, "LEAVE") == 0) {

                send_line(sd, "LEAVE\n");
                printf("Saliendo del modo juego.\n");
                return;

            }
            if((send(sd, buffer, sizeof(buffer),0) == -1)){

				perror("CLIENT ERROR. Function send().");
				exit(EXIT_FAILURE);

			}
        }
    }
}
