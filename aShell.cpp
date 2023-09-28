#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "sys/wait.h"
#include "unistd.h"

void read_command(char cmd[], char *par[]) {
    char line[1024];
    int count = 0, i = 0, j = 0;
    char *array[100], *pch;

    for (;;) {
        int c = fgetc(stdin);
        line[count++] = (char) c;
        if (c == '\n') {
            break;
        }
        if (count == 1) {
            return;
        }
        pch = strtok(line, "\n"); // break the lines into tokens (words)

        // parse the line into words
        while (pch != NULL)
        {
            array[i++] = strdup(pch);
            pch = strtok(NULL, "\n");
        }
        // first word is the command
        strcpy(cmd, array[0]);

        // others are parameters
        for (int j = 0; j < i; j++) {
            par[j] = array[j];
            par[i] = NULL; // NULL-terminate the parameter list
        }
    }
}

void type_prompt() {
    static int first_time = 1;
    if (first_time) {
        const char* CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
        write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
        first_time = 0;
    }

    printf("#");
}

int main() {
    char cmd[100], command[100], *parameters[20];

    char *envp[] = { (char *) "PATH=/bin", 0 };
    while(1) {
        type_prompt();
        read_command(command, parameters);
        if (fork() != 0) {
            wait(NULL);
        } else {
            strcpy(cmd, "/bin/");
            strcpy(cmd, command);
            execve(cmd, parameters, envp);
        }
        if (strcmp(command, "exit") == 0) {
            break;
        }
    }
    return 0;
}
