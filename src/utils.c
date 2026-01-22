#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <utils.h>

void clearStr(char *buffer) {
    size_t len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

char* extractCommand(char* input) {

    char* aux = input;
    char* command = strtok(aux, " ");
    return command;

}