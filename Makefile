CC = gcc
CFLAGS = -Wall -Iinclude

SRC = src/server.c src/client.c
OBJ = $(SRC:.c=.o)

all: server.exe client.exe

server.exe: src/server.c 
	$(CC) $(CFLAGS) -o server.exe src/server.c

client.exe: src/client.c
	$(CC) $(CFLAGS) -o client.exe src/client.c

clean:
	rm -f server.exe client.exe