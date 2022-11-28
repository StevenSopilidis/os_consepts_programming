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

    const int SIZE = 1024; // sise of shared channel
    const char *name = "OS"; // name of shared channel
    int fd;
    char *ptr;
    struct timeval current;

    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr, "fork() failed: {%d}\n", errno);
    } if (pid == 0) {
        gettimeofday(&current, NULL);
        fd = shm_open(name, O_CREAT | O_RDWR,0666);
        ftruncate(fd, SIZE);
        ptr = (char *) mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sprintf(ptr, "%lu", current.tv_usec);
        // combine all arguments passed and call exec
        execvp(argv[1], argv+2);
        return 0;
    } else if (pid > 1) {
        int status;
        wait(NULL);
        fd = shm_open(name, O_RDONLY, 0666);
        ptr = (char *) mmap(0, SIZE, PROT_READ, MAP_SHARED, fd, 0);
        int start_time = strtoll(ptr, NULL, 10);
        gettimeofday(&current, NULL);
        long long elapsed = current.tv_usec - start_time;
        printf("Elapsed=%lld\n", elapsed);
        shm_unlink(name);
        return 0;
    }
}