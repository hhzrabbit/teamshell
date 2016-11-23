// Simple Terminal / Shell in C

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

static void sighandler(int signo) {
  if (signo == SIGINT)
    { // ^C (signal 2)
      // printf("\nGoodbye, friend. [Closed by SIGINT]\n");
      exit(0);
    }
}

int main() {
  // Settings + Signals
  umask(0);
  signal (SIGINT, sighandler);

  char d[100];
  char * dest = d;
  char * command[32];
  int f = -1;
  
  // > LOOP HERE FOR TERMINAL
  while (1) {
    
    // Reading From Command Line
    fgets(dest, 256, stdin);
    //if (dest[strlen(dest)-1] = '\n')
    //  dest[strlen(dest)-1] = 0;
    char * a = strchr(
    // Parsing Commands
    int i = 0;
    while (dest) {
      command[i] = strsep(&dest, " ");
      i++;
    }
    command[i] = NULL;  
    // >> FORK HERE
    f = fork();
    if (f == 0) {    
      // Execute Command
      int j = execvp(command[0], command);
      exit(0);
    } else {
      wait();
    }
    // >>> FLUSH BUFFER HERE
    
    // >> END FORK

  }
  // > END LOOP

  // Exit
  return 0;
}

// Debugging Code Dump
/*  printf("%d\n", j);
    printf("%s\n", strerror(errno));*/
