CC = gcc
CFLAGS = -Wall -Iinclude

SERVER_SRC = src/server.c
SERVER_OBJ = $(SERVER_SRC:.c=.o)

CLIENT_SRC = src/client_main.c src/client_game.c src/utils.c
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

all: server.exe client.exe

server.exe: $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $(SERVER_OBJ)

client.exe: $(CLIENT_OBJ)
	$(CC) $(CFLAGS) -o $@ $(CLIENT_OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o server.exe client.exe
