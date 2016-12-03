// Simple Terminal / Shell in C
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "shell.h"
#include "parser.h"

//store absolute path to home dir
char home[1024];

//store the dup'd fd for stdin and stdout
int saveStdIn, saveStdOut = -1;

/* static void sighandler(int signo)
   interprets signals and handles accordingly
   args: the signal number for the signal
   return: nothing */
static void sighandler(int signo) {
  if (signo == SIGINT)
    { // ^C (signal 2)
      exit(0);
    }
}

/* void resetStdInOrOut(int fd, int type)
   (helper fxn) resets stdin/stdin to its standard place in file table
   args:   fd -- standard file descriptor of stdin/stdout
   type -- what to reset (stdin or stdout)
   0: stdin
   1: stdout
   return: nothing */
void resetStdInOrOut(int fd, int type){
  int a = dup2(fd, type);
}

/* void resetStdInOrOut(int fd, int type)
   resets the stdin and stdout to fd 0 and 1 respectively
   args: nothing
   return: nothing */
void resetStdIO(){
  if (saveStdIn != -1)
    resetStdInOrOut(saveStdIn, 0);
  if (saveStdOut != -1)
    resetStdInOrOut(saveStdOut, 1);
  saveStdIn = -1;
  saveStdOut = -1;
}


/* int redirect(char * redirectTo, int type)
   redirects where input comes from/output goes to
   args: redirectTo -- path to new input/output
   type -- what to redirect (input or output)
   0: redirect input
   1: redirect output 
   return: new file descriptor of stdin / stdout */
int redirect(char * redirectTo, int type){
  int fd;
  int fdToReplace;
  if (type == 0){ // <
    fd = open(redirectTo, O_CREAT | O_RDWR, 0644);
    fdToReplace = 0;
  }
  else if (type == 1){ // >
    fd = open(redirectTo, O_CREAT | O_RDWR | O_TRUNC, 0644);
    fdToReplace = 1;
  }
  else if (type == 2){ // >>
    fd = open(redirectTo, O_CREAT | O_RDWR | O_APPEND, 0644);
    fdToReplace = 1;
  }
  int ret = dup(fdToReplace);
  dup2(fd, fdToReplace);
  return ret;
}

/* int checkForRedirect(char ** command, int type)
   checks to see if redirection symbol in the command
   args: command -- array of strings representing the command entered
   type -- which redirection symbol to look for:
   0: < (redirect input)
   1: -> > (redirect output)
   return: index in the command array of the redirection symbol */
int checkForRedirect(char ** command, int type){
    char sign[2];
    char sign2[3];
    if (type == 0) {
      strncpy(sign, "<", 2);
    }
    else {
      strncpy(sign, ">", 2);
      strncpy(sign2, ">>", 3);
    }
    int index = 0;
    while(* command) {
      if (strcmp(* command, sign) == 0 || (type == 1 && strcmp(* command, sign2) == 0))
	return index;
      index++;
      command++;
    }
    return -1;
}
 
  /* char ** handleRedirects(char ** command)
     if there are > or < in the command, will redirect the stdin/stdout accordingly
     args: command -- array of strings representing the command entered
     return: command with a NULL in place of the < and/or > if found */
char ** handleRedirects(char ** command){
  int type; //type of redirection
  
  //redirInput is index of command where the < is
  int redirInput = checkForRedirect(command, 0);
  //redirInput is index of command where the > is
  int redirOutput = checkForRedirect(command, 1);
      
    if (redirInput != -1){
      type = 0;
      saveStdIn = redirect(command[redirInput + 1], type);
      command[redirInput] = 0;
    }
  
    if (redirOutput != -1){
      if (strcmp(command[redirOutput], ">") == 0)
	type = 1; 
      else // >>
	type = 2;
      saveStdOut = redirect(command[redirOutput + 1], type);
      command[redirOutput] = 0;
    }

    return command;
  }


  /* int checkForPiping(char ** command)
     checks to see if piping symbol in command
     args: command -- array of strings representing the command entered
     return: index of command array of the |, or -1 if not found*/
  int checkForPiping(char ** command){
    int index = 0;
    while(* command) {
      if (strcmp(* command, "|") == 0)
	return index;
      index++;
      command++;
    }
    return -1;
  }

  /* int pipedAndRan(char ** command)
     if pipe in the command array, pipes and executes commands
     args: command -- array of strings representing the command entered
     return: int for boolean value (1 if piped and ran, 0 if no pipe in command */
  int pipedAndRan(char ** command){
    //no piping to be done
    int index = checkForPiping(command);
    if (index == -1)
      return 0; //boolean false

    //pointer to beginning of command after the |
    char ** secondCommand = (command + index + 1);
    command[index] = 0;
  
    int pipefd[2];
    pipe(pipefd);

    int status;
    int j = -1;
  
    int fd = fork();
    if (fd == 0) { //chlild
      close(pipefd[0]);
      saveStdOut = dup(1);
      dup2(pipefd[1], 1);
      execute(command); // Execute First Command
      close(pipefd[1]);
      exit(0);
    }
    else {
      wait(&status);
      close(pipefd[1]);
      saveStdIn = dup(0);
      dup2(pipefd[0], 0);
      execute(secondCommand); // Execute Second Command
      close(pipefd[0]);
    }
    return 1; //boolean
  
  }

  /* void cd(char ** command)
     executes the command (change directory)
     args: command -- array of strings representing the command entered
     will be a cd command
     return: nothing */
  void cd(char ** command) {
    int res;
  
    //$ cd (takes you to home)
    if (! command[1]) 
      res = chdir(home);

    //$ cd <somewhere>
    else {
      res = chdir(command[1]);
    }

    if (res == -1)
      printf("No such directory\n");
  }

  /* char ** convertTildes(char ** command)
     changes any ~ in the command into the absolute path for the home directory
     args: command -- array of strings representing the command entered
     return: command with any ~ replaced */
  char ** convertTildes(char ** command){
    char ** ret = command;
    while (* command){
      if (* command[0] == '~') {
	char path[1024];
	strncpy(path, home, 1023);
	strncat(path, ++*command, 1023);
	* command = path;
      }
      command++;
    }
    return ret;
  }

  /* void execute(char ** command)
     forks and tries to execute command
     prints "<command>: command not found" if command entered does not exist.
     args: command -- array of strings representing the command entered
     return: nothing */
  void execute(char ** command) {
    // Execute Command
    int f = fork();
    int j = -1;
    int status;
    if (f == 0) {
    
      j = execvp(command[0], command);

      if (j == -1)
	printf("%s: command not found\n", command[0]);
      exit(0);
    } else {
      wait(&status);
    }
  }

  /* int main()
     forever loop: read input using fgets, prep input as needed, parse input into command, executes command
     args: nothing
     return: 0 (exit successfully)*/
  int main() {
    // Settings + Signals
    umask(0000);
    signal (SIGINT, sighandler);
    char cwd[1024];
  
    //save path to home
    strncpy(home, getenv("HOME"), 1024);
  
    char d[1024];
    char * dest = d;
    char ** pdest = &dest;
    char * cmd[1024];
    char ** command = cmd;
    int f = -1; // For Forking
    int i = 0; // For Parsing Commands
  
    // > LOOP HERE FOR TERMINAL
    while (1) {

      // Reading From Command Line

      //print prompt
      getcwd(cwd, sizeof(cwd));
      printf("%s> ", cwd);

      fgets(dest, 256, stdin);
      if (!*(dest+1)) continue;

      // Verify Input Parsable
      if (verify(dest) != 0) {
	printf("Encountered Bad Token: %s\n", dest);
	continue; 
      }

      // Preparsing
      strip(dest);
      dest[strlen(dest)+1] = 0; // Move terminating char one down
      dest[strlen(dest)] = ';'; // Add semicolon to end for efficient parsing


      // EXECUTION LOOP
      while (1) {
	if (!parseCommands(cmd, dest, pdest))
	  break;
	command = convertTildes(command);
	command = handleRedirects(command);

	//cd
	if (strcmp(cmd[0], "cd") == 0)
	  cd(command);
	//exit
	else if (strcmp(cmd[0], "exit") == 0) 
	  exit(0);
	//piping
	else if (! pipedAndRan(command)) {
	  execute(command);
	
	}
	resetStdIO();
      }
    }
    // > END LOOP
  
    // Exit
    return 0;
  }
