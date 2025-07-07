#include <client.h>

int main(int argc, char const *argv[])
{
    int socketDescriptor;
	struct sockaddr_in socketName;
	char buffer[250];
	socklen_t socketNameLength;
    fd_set readFds, auxFds;
    int end = 0;


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

    return 0;
}
