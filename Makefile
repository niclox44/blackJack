CC = gcc
CFLAGS = -Wall -Iinclude

SRC = src/server.c src/client.c
OBJ = $(SRC:.c=.o)

all: server client

server: src/server.c 
	$(CC) $(CFLAGS) -o server src/server.c

client: src/client.c
	$(CC) $(CFLAGS) -o client src/client.c

clean:
	rm -f server client