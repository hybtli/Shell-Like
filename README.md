# Shell-Like

Write a C program similar to a shell interface.

Your shell-like program displays the command line prompt “myshell>” and waits for the user's command. It reads the user’s next command,
parses it into separate tokens that are used to fill the argument vector for the command to be executed, and executes it.
Your shell-like program should support the following built-in commands ( which are not part of the
regular shell, the functionality is built directly into your shell itself ) :

**1) cd** <**directory**>: _change the current directory to <directory>_
  
**2) dir :** _print the current working directory_
  

**3) history :** _print 10 most recently entered commands in your shell_

**4) bye :** _terminate your shell process._
  

For any other commands, your shell-like program should consider them as system commands. For system commands, your program creates
a child process using fork system call, and the child process executes the command by using execvp() function ( You can assume that
the command does not include a pathname ).
  
          •    You need to handle both foreground and background processes for system commands.
               When a process runs in foreground, your program should wait for the task to complete,
               then immediately prompt the user for another command. A background process is indicated
               by placing an ampersand (’&’) character at the end of an input line. When a process run
               in background, your program should not wait for the task to complete, but immediately
               prompt the user for another command. 
  
          •    You are not allowed to use system() function.
 
Your shell-like program should support pipe operator between two processes.
  
          •    For a pipe in the command line, you need to connect stdout of the left command to stdin of
               the command following the "|". For example, if the user types "ls -al | sort", then the “ls”
               command is run with stdout directed to a Unix pipe, and that the sort command is run with
               stdin coming from that same pipe.
  
          •    You don't need to support multiple pipes.
  
