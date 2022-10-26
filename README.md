# Shell

A simple shell that handles three internal commands – ‘cd’, ‘echo’ and ‘pwd’ and five external commands – ‘ls’, ‘cat’, ‘date’, ‘rm’ and ‘mkdir’. 

External commands have been handled using two methods.
- Creating a new process using the fork() system call and using the execvp()call to run the individual program for the external command.
- Creating a thread using pthread_create() and system() call rather than execvp().

Two options per command have been provided. Refer to the descriptions folder for more information on the usage.
