#include "sys/types.h"
#include "unistd.h"
#include "stdio.h"
#include "string.h"

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 256

int main()
{
    char write_message[BUFFER_SIZE] = "Hallo child";
    char read_message[BUFFER_SIZE];
    int fd[2];
    pid_t pid;


    // create the pipe
    if(pipe(fd) == -1)
    {
        fprintf(stderr, "Could not create pipe");
        return 1;
    }

    pid = fork();
    if(pid < 0)
    {
        fprintf(stderr, "FORK failed");
        return 1;
    }
    if(pid > 0 )
    {
        // parent process
        close(fd[0]); // close unused end of pipe
        write(fd[1], write_message, strlen(write_message) + 1);
        close(fd[1]);
    } else {
        // child process
        close(fd[1]); // close the unused end of pipe
        read(fd[0], read_message, BUFFER_SIZE);
        printf("Message From pipe: %s\n", read_message);
        close(fd[0]);
    }
    return 0;
}