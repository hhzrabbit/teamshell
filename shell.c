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

char home[1024];

static void sighandler(int signo) {
  if (signo == SIGINT)
    { // ^C (signal 2)
      exit(0);
    }
}

void cd(char ** command){
  int res;
  
  //$ cd
  //takes you to home
  if (! command[1]) 
    res = chdir(home);

  //$ cd ~
  //takes you home
  else if (command[1][0] == '~'){
    char path[256];
    strcpy(path, home);
    strcat(path, ++command[1]);
    res = chdir(path);
  }

  //$ cd <somewhere>
  //takes you to <somewhere>
  else {
    res = chdir(command[1]);
  }

  if (res == -1)
    printf("No such directory\n");
  
}

int main() {
  // Settings + Signals
  umask(0);
  signal (SIGINT, sighandler);
  char cwd[1024];
  
  //save path to home
  getcwd(home, sizeof(cwd));
  
  char d[256];
  char * dest = d;
  char * command[256];
  int f = -1; // For Forking
  int i = 0; // For Parsing Commands
  
  // > LOOP HERE FOR TERMINAL
  while (1) {


    // Reading From Command Line
    getcwd(cwd, sizeof(cwd));
    printf("%s > ", cwd);
    fgets(dest, 256, stdin);
    if (dest[strlen(dest)-1] = '\n')
      dest[strlen(dest)-1] = 0;


    while (1) {
      // Parsing Commands
      i = 0;
      while (dest) {
	command[i] = strsep(&dest, " ");

	// >> SEMICOLON CHECK
	if (strcmp(command[i],";") == 0) {
	  break;
	}
	// >> END SEMICOLON CHECK
	i++;
      }
      command[i] = NULL;  

      //cd
      if (strcoll(command[0], "cd") == 0)
	cd(command);
      else {
	// >> FORK HERE
	f = fork();
	if (f == 0) {    
	  // Execute Command
	  int j = execvp(command[0], command);
	  exit(0);
	} else {
	  sleep(1);
	}
      }
      // >> END FORK
      if (!dest) {
	dest = d; // RESET BUFFER
	break;
      }
    }
  }
  
  // > END LOOP
  
  // Exit
  return 0;
}

// Debugging Code Dump
/*  printf("%d\n", j);
    printf("%s\n", strerror(errno));*/
