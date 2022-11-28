#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include "sys/time.h"
#include "sys/wait.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "must provide instruction to execture\n");
        return -1;
    }

    char *arguments = (char *)malloc(1024);
    int counter = 2;
    while(counter < argc) {
        char *arg = argv[counter++];
        strncat(arguments, arg, strlen(arg));
    }

    int fd[2];
    struct timeval current;

    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr, "fork() failed: {%d}\n", errno);
    } if (pid == 0) {
        gettimeofday(&current, NULL);
        close(fd[0]);
        char *message;
        sprintf(message, "%lu", current.tv_usec);
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
        // combine all arguments passed and call exec
        execvp(argv[1], argv+2);
        return 0;
    } else if (pid > 1) {
        int status;
        wait(NULL);
        close(fd[1]);
        char *message;
        read(fd[0], message, 1024);
        gettimeofday(&current, NULL);
        long long elapsed = current.tv_usec - strtoll(message, NULL, 10);
        printf("Elapsed=%lld\n", elapsed);
        return 0;
    }
}