#include <client.h>

int main(int argc, char const *argv[])
{
    int socketDescriptor;
	struct sockaddr_in socketName;
	char buffer[250];
	socklen_t socketNameLength;
    fd_set readFds, auxFds;
    int status = !EXIT;


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

	printf("Conectado al servidor.\n");

	do{

		
		puts("Teclee el mensaje que desea enviar.");
		fgets(buffer,sizeof(buffer),stdin);
		buffer[strlen(buffer)-1] = '\0';

		if(!strcmp(buffer,"SALIR")){

			status = EXIT;
		}

		/*GOING to SEND the message from the SERVER*/

		if(send(socketDescriptor, buffer, sizeof(buffer),0) == -1){
			perror("CLIENT ERROR. Function send().");
			exit(EXIT_FAILURE);
		}

		/*GOING to RECIVE the message from the SERVER*/

		bzero(buffer, sizeof(buffer));

		if(recv(socketDescriptor,buffer,sizeof(buffer),0) == -1){
			perror("CLIENT ERROR. Function recv().");
			exit(EXIT_FAILURE);
		}

		printf("Respuesta del servidor: %s\n",buffer);



	}while( status != EXIT );

	close(socketDescriptor);


    return 0;
}
