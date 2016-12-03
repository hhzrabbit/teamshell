/* static void sighandler(int signo)
interprets signals and handles accordingly
args: the signal number for the signal
return: nothing */
static void sighandler(int signo);

/* void resetStdInOrOut(int fd, int type)
(helper fxn) resets stdin/stdin to its standard place in file table
args:   fd -- standard file descriptor of stdin/stdout
      type -- what to reset (stdin or stdout)
        0: stdin
	1: stdout
return: nothing */
void resetStdInOrOut(int fd, int type);

/* void resetStdInOrOut(int fd, int type)
resets the stdin and stdout to fd 0 and 1 respectively
args: nothing
return: nothing */
void resetStdIO();


/* int redirect(char * redirectTo, int type)
redirects where input comes from/output goes to
args: redirectTo -- path to new input/output
            type -- what to redirect (input or output)
              0: redirect input
              1: redirect output 
return: new file descriptor of stdin / stdout */
int redirect(char * redirectTo, int type);

/* int checkForRedirect(char ** command, int type)
checks to see if redirection symbol in the command
args: command -- array of strings representing the command entered
         type -- which redirection symbol to look for:
             0: < (redirect input)
             1: -> > (redirect output)
return: index in the command array of the redirection symbol */
int checkForRedirect(char ** command, int type);

/* char ** handleRedirects(char ** command)
if there are > or < in the command, will redirect the stdin/stdout accordingly
args: command -- array of strings representing the command entered
return: command with a NULL in place of the < and/or > if found */
char ** handleRedirects(char ** command);

int checkForPiping(char ** command);

int pipedAndRan(char ** command);

/* void cd(char ** command)
executes the command (change directory)
args: command -- array of strings representing the command entered
      will be a cd command
return: nothing */
void cd(char ** command);

/* char ** convertTildes(char ** command)
changes any ~ in the command into the absolute path for the home directory
args: command -- array of strings representing the command entered
return: command with any ~ replaced */
char ** convertTildes(char ** command);

/* void execute(char ** command)
forks and tries to execute command
prints "<command>: command not found" if command entered does not exist.
args: command -- array of strings representing the command entered
return: nothing */
void execute(char ** command);
