#include "unistd.h"
#include "stdio.h"
#include "sys/types.h"
#include "errno.h"
#include "stdlib.h"

int main() {
  pid_t pid;
  pid = fork();
  if(pid < 0) {
  	fprintf(stderr, "fork() failed\n");
  } else if (pid > 0) {
	sleep(30);
  	exit(0);
  }else if (pid == 0) {
  	exit(0);
  }
}
