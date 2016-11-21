#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(){
  char d[100];
  char * dest = d;
  printf("What would you like to do?: ");
  fgets(dest, 100, stdin);


  char * command[10];
  int i = 0;
  while (dest) {
    command[i] = strsep(&dest, " ");
    printf("command[i]: %s\n", command[i]);
    i++;
  }

  command[i-1][strlen(command[i-1]) - 1] = 0;
  command[i] = NULL;
  int j = execvp(command[0], command);
  printf("%d\n", j);
  printf("%s\n", strerror(errno));
  return 0;
}
