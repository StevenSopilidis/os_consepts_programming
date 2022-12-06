#include "sys/types.h"
#include "sys/wait.h" 
#include "sys/shm.h"
#include "sys/mman.h"
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "errno.h"

#define MAX_COMMAND_LENGTH 80
#define MAX_AGUMENTS 20
#define SHM_SIZE 1
#define SHM_NAME "SHELL_SHM"
#define SHELL_HISTORY_SIZE 1024
#define MAX_WORKING_DIR_SIZE 1024

typedef struct  {
    int commandExecuted;
    char** commands;
} SHELL_HISTORY;

void get_command(char* buffer) {
    int c;
    int i = 0;
    while ((c = getchar()) != '\n' && c != EOF) { 
        *(buffer + i) = c;
        i++;
    }
}
char* get_instruction(char* command,int* consumed) {
    int i = 0;
    char* instruction = (char *)malloc(MAX_COMMAND_LENGTH);
    while(*(command + i) != ' ' && *(command + i) != '\n' && *(command + i) != '\0')
    {
        *(instruction + i) = *(command + i);
        i++;
    }
    *(instruction + i) = '\0';
    *consumed = i;
    return instruction;
}
int get_arguments(char* command,char* instruction, int consumed, char** args)
{
    char* dest= (char *)malloc(MAX_COMMAND_LENGTH);
    strcat(dest, "/bin/");
    strcat(dest, instruction);
    *(args) = dest;
    // convert char* ---> char* [] in order to provide args to execvp
    char* arg_string = command + consumed + 1; // +1 to skip space
    int counter = 1;
    /* Establish string and get the first token: */
    char *arg = (char *)malloc(MAX_COMMAND_LENGTH + 1);
    arg = strtok(arg_string, " ");
    while( arg != NULL )
    {
        /* While there are tokens in "string" */
        *(args + counter) = arg;
        /* Get next token: */
        arg = strtok( NULL, " ");
        counter++;
    }
    return counter;
}
void print_history(SHELL_HISTORY* history)
{
    for (int i = 0; i < history->commandExecuted; i++)
    {
        printf("---> %s\n", history->commands[i]);
    }
}

int main(int argc, char* argv[]) 
{
    char* command_buffer;
    char* historyBuffer[SHELL_HISTORY_SIZE];
    SHELL_HISTORY history = {
        .commandExecuted = 0,
        .commands = historyBuffer
    };
    char cwd[256];

    while(1)
    {
        command_buffer = (char *)malloc(MAX_COMMAND_LENGTH);
        getcwd(cwd, sizeof(cwd));
        printf("<%s>", cwd);
        get_command(command_buffer);
        pid_t pid = fork();
        if(pid < 0) {
            fprintf(stderr, "fork() failed\n");
            exit(-1);
        } else if(pid == 0) { // chilld process
            if(!strcmp(command_buffer, "!!!")) // use wants to check history
            {
                printf("History: \n");
                print_history(&history);
                printf("---------\n");
                exit(0);
            } else {
                int consumed;
                char** args = (char **)malloc(MAX_AGUMENTS * (sizeof(char *)));
                // get the commmand & arguments
                char* instruction = get_instruction(command_buffer, &consumed);
                int args_len = get_arguments(command_buffer,instruction, consumed, args);

                if(!strcmp(instruction, "<") || !strcmp(instruction, ">"))
                {
                    printf("Invalid command\n");
                    exit(0);
                }
                
                // loop arguments and check for redirection
                // skip command file
                for(int i = 1; i < args_len; i++)
                {
                    int fd;
                    if(!strcmp("<", args[i]))
                    {
                        // check if file specified on the left
                        if(i == args_len - 1)
                        {
                            printf("Invalid input\n");
                            exit(0);
                        }
                        fd = open(args[i + 1], O_RDONLY, 0);
                        dup2(fd, STDIN_FILENO);
                        args+=2;
                        i+=2;
                    }else if(!strcmp(">", args[i]))
                    {
                        if(i == args_len - 1)
                        {
                            printf("Invalid input\n");
                            exit(0);
                        }
                        fd = creat(args[i + 1], 0644);
                        dup2(fd, STDOUT_FILENO);
                        args+=2;
                        i+=2;
                    }
                }
                
                // if last char is & remove it since it just 
                // interests thet parent
                char last_char = command_buffer[strlen(command_buffer) -1];
                if(last_char == '&')
                {
                    //remove it from last instruction
                    char* arg = args[args_len - 1];
                    arg[strlen(arg) - 1] = '\0';
                }
                int result = execvp(instruction, args);
                if(result < 0) {
                    fprintf(stderr, "Error: %s\n", strerror(errno));
                }
            }
        } else { // parent process
            int consumed;
            char* instruction = get_instruction(command_buffer, &consumed);
            if(!strcmp(instruction, "cd"))
            {
                int result = chdir(command_buffer + 3);
                if(result < 0) 
                {
                    fprintf(stderr, "Error: %s\n", strerror(errno));
                }
            }else {
                // check wether or not we should wait for child to finish
                char last_char = command_buffer[strlen(command_buffer) - 1];
                if(last_char != '&')
                    wait(NULL);
                // save command in history if we have space
                if(history.commandExecuted < SHELL_HISTORY_SIZE && strcmp(command_buffer, "!!!") != 0)
                {
                    int commandsExecuted = history.commandExecuted;
                    history.commands[commandsExecuted] = (char *)malloc(MAX_COMMAND_LENGTH);
                    strcpy(history.commands[commandsExecuted], command_buffer);
                    history.commandExecuted++;
                }
                free(command_buffer);
                command_buffer = (char *)malloc(MAX_COMMAND_LENGTH);
            }
        }
        fflush(stdout);
    }
}
