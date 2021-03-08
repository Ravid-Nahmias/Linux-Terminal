ravidna
Ravid Nahmias, 316032515

Description of the program:
The program symulates a shell system.
like a real shell system, this program display an open line- the prompt line which show the user name and 
then the curren directory.
then the user can insert the desirable command, and the program will handle it.
currently, the program doesnt know how to handle the command "cd" so the output will be in accordance.
the program can handle the signal SIGTSTP if the user pressed ctrl+z, in case like that the 
command that inserted before will stop but the parent process will continue working.
In addition, we have and option of the "bg" command that will revive the last command that paused.

The program has one file:
ex3a.c - this file has the entire code, the functions+ the main. 


If you want to compile the program:
gcc -g ex3a.c -o shell -Wall -Wvla -lpthread

If you want to run the program:
./shell

Input orders:
enter the desirable command to execute.

The program will work until the user will insert the word "done", 
the program will output a summary lines and then exit the program.


