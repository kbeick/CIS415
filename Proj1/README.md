CIS415 Project 1
======
:author: Kevin Beick

:source files:
* ArrogantShell.c -- source code for shell's main functionality
* makefile --------- use to easily compile code
* helloSysCall.c --- creates a system call with the name *sys_helloworld()*
* runHello.c ------- used to run my new system call

###Extra Credit:
- I implemented an input parser to handle program arguments and pass them to execve().
- From the signal(2), signal(7) man pages (http://man7.org/linux/man-pages/man7/signal.7.html):
    - SIGSTOP and SIGKILL cannot be caught or ignored.
    - The following signals terminate the process by default (and may additionally dump core):
    signals 1-16,24-27,29-31

###Compilation Instructions:
cd into the project directory. The enter 'make' into the terminal to compile my shell. The executable is called 'arrogantshell' so enter ./arrogantshell to run it. Optionally include a number of seconds to be used as a timeout as a commandline argument.

###Project Overview:
I made my own shell that takes input from the commandline and parses it into command and arguments. The shells then spawns a child process and attempts to execute the user-given command.
The user has the option of including a program timeout as a commandline argument. For example, if executed as <./arrogantshell 10>, the shell will kill any executing command that takes longer than 10 seconds and make fun of you.
I also created my own new system call that would print 'hello world!' to the kernel.  I was successful in compiling a new kernel that included this system call, but was unable to successfully call it; the system call only returned -1.

###Code Description and Layout:
The source code for the shell is all contained in ArrogantShell.c.
The shell is basically an infinite loop that reads in user commands+arguments. It then parses the input, forks a new child process, and attempts to run the user's command in that child process. If the user supplied a timeout as a commandline argument when starting the shell, then if a command takes longer than allowed the shell kills the process and makes fun of the user. On the other hand, if a command successfully terminates before the timeout, then the shell sulks.

###General Comments:
I'm not sure why my system call wasn't able to be successfully called. Like others in the class (at least based off posts on Piazza and comments in class), I successfully compiled a new kernel that included my new system call, but my test program wasn't able to call it successfully. I checked it's number in the system call table, and double checked that I followed all the steps in the guide correctly.
oh well...

~18 hours spent on project total.
