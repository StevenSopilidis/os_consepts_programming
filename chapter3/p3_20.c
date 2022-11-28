#include "stdio.h"
#include "stdlib.h"

#define MIN_PID 300
#define MAX_PID 5000

typedef int Pid;

typedef struct {
    int number;
    int available;
} pid_struct_entry;

pid_struct_entry *pid_struct;

int allocate_map(void) {
    pid_struct = (pid_struct_entry *)malloc(sizeof(pid_struct_entry) * (MAX_PID - MIN_PID));
    if(pid_struct == NULL)
    {
        fprintf(stderr, "Could not initialize structure\n");
        return -1;
    }
    for (int i = 0; i < MAX_PID - MIN_PID; i++)
    {
        pid_struct[i].available = 1;
        pid_struct[i].number = MIN_PID + i;
    }
    return 1;
}
Pid allocate_pid() {
    for (size_t i = 0; i < MAX_PID - MIN_PID; i++)
    {
        if(pid_struct[i].available == 1) {
            pid_struct[i].available = 0;
            return pid_struct[i].number;
        }
    }
    return -1;
}
void release_pid(Pid pid) {
    pid_struct[pid - MIN_PID].available = 1;
}

int main() {
    int allocated = allocate_map();
    if(allocated == -1)
        return -1;
    printf("%d\n", allocate_pid());
    printf("%d\n", allocate_pid());
    printf("%d\n", allocate_pid());
    release_pid(300);
    printf("%d\n", allocate_pid());
    return 0;
}