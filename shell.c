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

//checks to see if redirection in the command
//type == 0 -> <
//type == 1 -> >
int checkForRedirect(char ** command, int type){
  char sign[2];
  if (type == 0)
    strncpy(sign, "<", 2);
  else 
    strncpy(sign, ">", 2);
  int index = 0;
  while(* command) {
    if (strcmp(* command, sign) == 0)
      return index;
    index++;
    command++;
  }
  return -1;
}

//type == 0 -> redirect input
//type == 1 -> redirect output
//returns file descriptor to stdin / stdout that was redirected
int redirect(char * redirectTo, int type){
  int fd;
  int fdToReplace;
  if (type == 0){
    fd = open(redirectTo, O_RDONLY);
    fdToReplace = 0;
  }
  else {
    fd = open(redirectTo, O_WRONLY);
    fdToReplace = 1;
  }
  int ret = dup(fdToReplace);
  dup2(fd, fdToReplace);
  return ret;
}

//type == 0 -> stdin
//type == 1 -> stdout
void resetStdInOrOut(int fd, int type){
  dup2(fd, type);
}

int execute(char ** command) {
  // Execute Command
  int f = fork();
  int j = -1;
  if (f == 0) {
    int redirOutput = checkForRedirect(command, 0);
    int redirInput = checkForRedirect(command, 1);
    int saveStdIn, saveStdOut = -1;
    if (redirInput != -1){
      saveStdIn = redirect(command[redirInput + 1], 0);
      command[redirInput] = 0;
    }

    if (redirOutput != -1){
      saveStdOut = redirect(command[redirOutput + 1], 1);
      command[redirOutput] = 0;
    }    

    j = execvp(command[0], command);

    if (saveStdIn != -1)
      resetStdInOrOut(saveStdIn, 0);
    if (saveStdOut != -1)
      resetStdInOrOut(saveStdOut, 1);
    
    if (j == -1)
      printf("Invalid command.\n");
    exit(0);
  } else {
    wait();
  }
  return j;
}


void cd(char ** command) {
  int res;
  
  //$ cd (takes you to home)
  if (! command[1]) 
    res = chdir(home);

  //$ cd ~ (takes you home)
  //$ cd ~/<somewhere> 
  else if (command[1][0] == '~'){
    char path[256];
    strcpy(path, home);
    strcat(path, ++command[1]);
    res = chdir(path);
  }

  //$ cd <somewhere>
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
      if (strcmp(command[0], "cd") == 0)
	cd(command);
      //exit
      else if (strcmp(command[0], "exit") == 0) 
	exit(0);
      else {
	  int j = execute(command);
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
