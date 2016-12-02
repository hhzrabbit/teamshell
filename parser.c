// Parsing in C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Headers
void strip(char * in);
char * parseCommands(char ** cmds, char * in, char ** parent);

// Functions
/* void strip(char * in)
 * takes in a string and removes all leading and trailing whitespace (modifies the string)
 * args: string in
 * return: <void> -- modifies string
 */
void strip(char * in) {
  char * tmp = in;
  char * front = 0;
  char * end = 0;
  while (*tmp) {
    if (!front && !(*tmp == ' ' || *tmp == '\t' || *tmp == '\n'))
      front = tmp;
    if (!(*tmp == ' ' || *tmp == '\t' || *tmp == '\n' || *tmp == ';'))
      end = tmp;
    tmp++;
  }
  if (!front || !end) return; // Checks for pure whitespace or empty string
  while (front < end + 1) {
    *in = *front;
    front++; in++;
  }
  *in = 0;
}

/* void parseCommands(char ** cmd, char * in, char ** parent)
 * parses a string of input to an array of commands, and modifies the string to allow for future
 * parsing (splits on semicolons)
 * args: char ** cmd    - array of strings, representing arguments for execvp 
 *       char *  in     - input string with unparsed command line arguments
 *       char ** parent - pointer to in to allow modification
 * return: char * - returns the new value of in
 */
char * parseCommands(char ** cmd, char * in, char ** parent) {
  // Pre-parsing
  char * dest = strsep(&in, ";");
  strip(dest); // Command, Ready For Parsing 
  if (dest == NULL || in == NULL) return 0; // No Commands

  // Parsing into cmd
  int i = 0;
  while (dest) {
    cmd[i] = strsep(&dest, " ");
    if (strcmp(cmd[i], "") == 0) i--; // Ignore extra spaces
    i++;
  }
  cmd[i] = NULL;
  *parent = in;
  return in;
}
