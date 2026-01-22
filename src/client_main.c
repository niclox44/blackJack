#include <client.h>
#include <utils.h>

int main(int argc, char const *argv[])
{
    int socketDescriptor, serverValue;
	struct sockaddr_in socketName;
	char buffer[250], aux[250];
	socklen_t socketNameLength;
    fd_set readFds, auxFds;
    int status = 1, salida = 0;


    socketDescriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (socketDescriptor == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    	exit(1);	
	}



    socketName.sin_family = AF_INET;
	socketName.sin_port = htons(2060);
	socketName.sin_addr.s_addr =  inet_addr("127.0.0.1");

	

    socketNameLength = sizeof(socketName);
	
	if (connect(socketDescriptor, (struct sockaddr *)&socketName, socketNameLength) == -1)
	{
		perror ("Error de conexión");
		exit(1);

	}

	FD_ZERO(&auxFds);
    FD_ZERO(&readFds);
    
    FD_SET(0,&readFds);
    FD_SET(socketDescriptor,&readFds);
	
	printf("+Ok. Usuario conectado.\n");

	printf("===================== \n");
	printf("| OPCIONES DE SESION | \n");
	printf("=====================\n");
	printf("- Salir-> 'SALIR'.\n");
	printf("- Iniciar sesión-> INICIO -u <nombre_usuario> -p <contraseña>\n");
	printf("- Registrarse -> 'REGISTRO -u <nombre_usuario> -p <contraseña>'\n");
	printf("\n");

	do{

		auxFds = readFds;
    	salida = select(socketDescriptor+1,&auxFds,NULL,NULL,NULL);
		if(salida < 0) status = EXIT;

		/*Check if there is data to read from the server*/
		if(FD_ISSET(socketDescriptor, &auxFds)){

			bzero(buffer, sizeof(buffer));
			serverValue = recv(socketDescriptor,buffer,sizeof(buffer),0);
			
			if(serverValue == -1){

				perror("CLIENT ERROR. Function recv().");
				exit(EXIT_FAILURE);

			}else if (serverValue == 0){

				printf("-SERVER ERROR. Server has been disconnected.\n");
				status = EXIT;

			}else{

				clearStr(buffer);

				if(strcmp(buffer,"Usuario autenticado correctamente.") == 0){

					printf("Respuesta del servidor: %s\n",buffer);
					printf("Entrando en el modo juego...\n");

					gameLoop(socketDescriptor);

					printf("==================== \n");
					printf("|OPCIONES DE SESION| \n");
					printf("====================\n");
					printf("- Salir-> 'SALIR'.\n");
					printf("- Iniciar sesión-> INICIO -u <nombre_usuario> -p <contraseña>\n");
					printf("- Registrarse -> 'REGISTRO -u <nombre_usuario> -p <contraseña>'\n");
					printf("\n");

				}else{
					printf("Respuesta del servidor: %s\n",buffer);
				}

			}

		}else{

			/*Read the command*/
			if(FD_ISSET(0, &auxFds)){

				bzero(buffer, sizeof(buffer));
				bzero(aux, sizeof(aux));
				fgets(buffer,sizeof(buffer),stdin);
				clearStr(buffer);

				strcpy(aux, buffer);
				char* util = NULL;
                char* command = strtok_r(aux, " ", &util);



				if(strcmp(command, "REGISTRO") != 0 && strcmp(command, "INICIO") != 0 && strcmp(command, "SALIR") != 0){

					printf("CLIENT ERROR. Comando no válido.\n");
					continue;

				}else{
					if((send(socketDescriptor, buffer, sizeof(buffer),0) == -1)){

						perror("CLIENT ERROR. Function send().");
						exit(EXIT_FAILURE);

					}

					if(strcmp(buffer,"SALIR") == 0){

						printf("Comando para salir.\n");
						status = EXIT;

					}
				}
			

			}
				

		}

	}while( status != EXIT);

	close(socketDescriptor);


    return 0;
}
