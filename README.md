# teamshell
### by Haley Zeng, Zicheng Zhen and Vandana Agarwala: Systems with DW, Period 4

### Features
* forks and executes each command inputted
* executes multiple commands sent at once that are separated by semicolons
	* does not require semicolons to have space padding
	* detects unexpected tokens (leading `;`, `;;`, `;&`, `&;`)
* cd and ls (not forked)
* exits at SIGINT
	* will not exit if the shell is running another program
* redirects > and <
* pipes (only single pipe)
* understands ~ to mean the home directory
* will not break if command entered has extraneous whitespace

---

### Unsuccessful Features
* Clearing (CTRL+L) and being able to access last statement using up key.
* Multiple pipes in one command.

---

### Bugs

---

### Function Headers

__shell.c__ - where all the action happens, and the heart of the shell

```static void sighandler(int signo)```

interprets signals and handles accordingly

* args: `int signo` -- the signal number for the signal
* return: `void`

```int checkForRedirect(char ** command, int type)```

checks to see if redirection symbol (`>`, `<`) in the command

* args:
  * `char ** command` -- array of strings representing the  command entered
  * `int type` -- which redirection symbol to look 
	  * 0: < (redirect input)
      * 1: > (redirect output)
* return: index in the command array of the redirection symbol

```int redirect(char * redirectTo, int type)```

redirects where input comes from/output goes to

* args:
  * `char * redirectTo` -- path to new input/output
  * `int type` -- what to redirect (input or output)
	    * 0: redirect input
	    * 1: redirect output 
* return: new file descriptor of stdin / stdout

```void resetStdInOrOut(int fd, int type)```

(helper fxn) resets stdin/stdin to its standard place in file table

* args:
  * `int fd` -- standard file descriptor of stdin/stdout
  * `int type` -- what to reset (stdin or stdout)
	  * 0: stdin
	  * 1: stdout
* return: `void`

```void resetStdInOrOut(int fd, int type)```
resets the stdin and stdout to fd 0 and 1 respectively
* args: 
* return: `void`

```char ** handleRedirects(char ** command)```

if there are `>` or `<` in the command, will redirect the stdin/stdout accordingly

* args: `char ** command` -- array of strings representing the command entered
* return: command with a NULL in place of the < and/or > if found */

```int checkForPiping(char ** command)```

checks to see if piping symbol in command

* args: `char ** command` -- array of strings representing the command entered
* return: index of command array of the |, or -1 if not found

```int pipedAndRan(char ** command)```

if pipe in the command array, pipes and executes commands

* args: `char ** command` -- array of strings representing the command entered
* return: int for boolean value (1 if piped and ran, 0 if no pipe in command

```void cd(char ** command)```

executes the command (change directory)

* args: `char ** command` -- array of strings representing the command entered (will be a cd command)
* return: `void`

```char ** convertTildes(char ** command)```

changes any ~ in the command into the absolute path for the home directory

* args: `char ** command` -- array of strings representing the command entered
* return: command with any `~` replaced

```void execute(char ** command)```

forks and tries to execute command, prints "`<command>`: command not found" if command entered does not exist.

* args: `char ** command` -- array of strings representing the command entered
* return: `void`

```int main()```

forever loop: read input using fgets, prep input as needed, parse input into command, executes command

* args: 
* return: 0 (exit successfully)

---

__parser.c__ - parsing utility to make the main loop in shell cleaner

```void strip(char * in)```

takes in a string and removes all leading and trailing whitespace (modifies the string)

* args: 
	* `char * in` -- input string from stdin
* return: `void` -- modifies string

```void parseCommands(char ** cmd, char * in, char ** parent)```

parses a string of input to an array of commands, and modifies the string to allow for future parsing (splits on semicolons)

* args: 
  * `char ** cmd` -- array of strings, representing arguments for execvp 
  * `char *  in` -- input string with unparsed command line arguments
  * `char ** parent` -- pointer to in to allow modification
* return: `char *` -- returns the new value of in

```char * verify(char * in)```

checks to make sure there are no unexpected tokens (leading `;`, `;;`, `;&`, `&;`) written in terminal

* args:
	* `char * in` -- unparsed input string
* return: `char *` -- modifies `in` to an unexpected token if it exists, returns `null` otherwise