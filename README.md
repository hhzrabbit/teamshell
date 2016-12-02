# teamshell
### by Vandana Agarwala, Haley Zeng, and Zicheng Zhen: Systems with DW, Period 4

### Features
* forks and executes each command inputted
* executes multiple commands sent at once that are separated by ;
* cd and ls (not forked)
* exits at SIGINT
* redirects > and <
* understands ~ to mean the home directory
* will not break if command entered has extraneous spaces

---

### Unsuccessful Features

---

### Bugs

---

### Function Headers

__shell.c__

```static void sighandler(int signo)```

interprets signals and handles accordingly

* args: signo -- the signal number for the signal
* return: nothing

```int checkForRedirect(char ** command, int type)```

checks to see if redirection symbol in the command

* args:
  * command -- array of strings representing the  command entered
  * type -- which redirection symbol to look for:
             0: < (redirect input)
             1: -> > (redirect output)
* return: index in the command array of the redirection symbol

```int redirect(char * redirectTo, int type)```

redirects where input comes from/output goes to

* args:
  * redirectTo -- path to new input/output
  * type -- what to redirect (input or output)
    * 0: redirect input
    * 1: redirect output 
* return: new file descriptor of stdin / stdout

```void resetStdInOrOut(int fd, int type)```

(helper fxn) resets stdin/stdin to its standard place in file table

* args:
  * fd -- standard file descriptor of stdin/stdout
  * type -- what to reset (stdin or stdout)
    * 0: stdin
    * 1: stdout
* return: nothing

```void resetStdInOrOut(int fd, int type)```
resets the stdin and stdout to fd 0 and 1 respectively
* args: nothing
* return: nothing

`char ** handleRedirects(char ** command)```

if there are > or < in the command, will redirect the stdin/stdout accordingly

* args: command -- array of strings representing the command entered
* return: command with a NULL in place of the < and/or > if found */

```void cd(char ** command)```

executes the command (change directory)

* args: command -- array of strings representing the command entered (will be a cd command)
* return: nothing

```char ** convertTildes(char ** command)```

changes any ~ in the command into the absolute path for the home directory

* args: command -- array of strings representing the command entered
* return: command with any ~ replaced

```void execute(char ** command)```

forks and tries to execute command, prints "<command>: command not found" if command entered does not exist.

* args: command -- array of strings representing the command entered
* return: nothing

```int main()```

forever loop: read input using fgets, prep input as needed, parse input into command, executes command

* args: nothing
* return: 0 (exit successfully)

__parser.c__

```void strip(char * in)```

takes in a string and removes all leading and trailing whitespace (modifies the string)

* args: string in
* return: <void> -- modifies string

```void parseCommands(char ** cmd, char * in, char ** parent)```

parses a string of input to an array of commands, and modifies the string to allow for future parsing (splits on semicolons)

* args: 
  * char ** cmd -- array of strings, representing arguments for execvp 
  * char *  in -- input string with unparsed command line arguments
  * char ** parent -- pointer to in to allow modification
* return: char * -- returns the new value of in
