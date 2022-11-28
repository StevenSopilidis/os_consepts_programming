#include "stdlib.h"
#include "wait.h"
#include "stdio.h"
#include "unistd.h"
#include "sys/types.h"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        fprintf(stderr, "N must be provided\n");
        return -1;
    }
    int n = atoi(argv[1]);
    pid_t pid = fork();
    if(fork < 0) {
        fprintf(stderr, "fork() failed");
        return -1;
    } else if (fork == 0) { // child process
        while (n != 1)
        {
            if(n % 2 == 0)
                n = n/2;
            else
                n = 3*n+1;
            printf("N=%d\n",n);
        } 
    } else { // parent process
        wait(NULL);
        return 0;
    }
}