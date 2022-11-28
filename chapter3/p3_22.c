#include "sys/types.h"
#include "sys/shm.h"
#include "sys/mman.h"
#include "sys/wait.h"
#include "unistd.h"
#include "stdio.h"
#include "stdlib.h"
#include "fcntl.h"

#define SHM_SIZE 1024 * sizeof(int)
#define SHM_NAME "shm_name"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Please providen n\n");
        return -1;
    }
    int fd;
    char *ptr;
    int num = atoi(argv[1]);
    printf("N = %d\n", num);
    pid_t pid = fork();
    if(pid < 0) {
        fprintf(stderr, "fork() failed\n");
        return -1;
    } else if(pid == 0)  { // child process
        fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
        ftruncate(fd, SHM_SIZE);
        ptr = (char *)mmap(0, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
        while (num != 1)
        {
            if(num % 2 == 0)
                num = num/2;
            else
                num = 3*num+1;
            sprintf(ptr,"%s %d", ptr, num);
        }
    } else if(pid > 0) {
        wait(NULL);
        fd = shm_open(SHM_NAME, O_RDONLY, 0666);
        ptr = (char *)mmap(0, SHM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
        printf("Result: %s\n", ptr);
        shm_unlink(SHM_NAME);
    }
}