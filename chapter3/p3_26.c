#include "sys/types.h"
#include "sys/wait.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define READ_END 0
#define WRITE_END 1

char* reverse_case(char* string) {
    char *result = (char *)malloc(strlen(string));
    int counter = 0;
    while(*string != '\0') {
        if(*string != ' ') 
        {
            if(*string <= 90)
                *(result + counter) = *string + 32;
            else 
                *(result + counter) = *string - 32;
        } else 
            *(result + counter) = ' ';
        string++;
        counter++;
    }
    *(result+counter) = '\0';
    return result;
}

int main() {
    char* message = "Halllo There My FriEND";
    int fd[2];
    if(pipe(fd) == -1) {
        fprintf(stderr, "pipe() failed\n");
        return -1;
    }
    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr, "fork() fork failed\n");
        return -1;
    } else if(pid == 0) { // child process
        char* to_send = reverse_case(message);
        close(fd[READ_END]);
        write(fd[WRITE_END], to_send, strlen(to_send)+1);
        close(fd[WRITE_END]);
    } else { // parent process
        close(fd[WRITE_END]);
        char *buffer = (char *)malloc(1024);
        read(fd[READ_END], buffer, 1024);
        printf("Result: %s\n", buffer);
        close(fd[READ_END]);
    }
}