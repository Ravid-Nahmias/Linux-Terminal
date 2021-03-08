#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <signal.h>
pid_t pid;

    void promptLine(){
        // this method is for the prompt line
        struct passwd *pw; // will hold the user name
        char *cwd; // will hold the path to current directory
        pw = getpwuid(getuid());
        cwd = getcwd( NULL, 0 );    // get the path

        printf("%s@%s>", pw->pw_name, cwd); //print the open line
        free(cwd);
    }
    void splitTheCommand(char* command, char** command_words, int *idx){           
        // split the command to comman name, arguments
        char* argument= strtok(command, " ");  //copy the first argument(command name) to an array
        *(idx)=0;
        int arg_size=0;
        while(argument != NULL){ //fill the array command_words with the arguments
            arg_size=strlen(argument);   
            command_words[*idx]=(char*)malloc(sizeof(char)*(arg_size+1));//allocate an array for the argument 
            if(command_words[*idx]== NULL){     // check that the memory allocated
                printf("An array for part of the command didnt allocate");
                exit(1);
            }
            strcpy(command_words[*idx], argument); //copy the argument
            (*idx)++;
            argument=strtok(NULL, " ");
        }
        command_words[*idx]=NULL;  // if its the end of the command
        free(argument); 
    }
    void printSummary(int length, int counter){
        // printing all the summary lines
        printf("Num of commands: %d\n",counter);
        printf("Total length of all commands: %d\n",length);
        double average=((double)length/(double)counter);
        printf("Average length of all commands: %lf\n",average);
        printf("See you next time!\n");
        exit(0);
    }
    void sigTSTP_handler(int sigNum){
        kill(pid,SIGTSTP);
    }
    void sigCHLD_handler(int sigNum){
        waitpid(-1,NULL,WNOHANG);
    }
    void execTheCommand(char** command_words){
        pid_t x= fork();    // create a thread that will run the command
            if(x<0){
                printf("couldnt create new process\n");
                exit(1);
            }
            else if(x==0){      // son process
                setpgrp();      // pull it out of the parent group so he will act the way we want
                signal(SIGTSTP,SIG_DFL);        
                execvp(command_words[0], command_words);    //activate the command
                exit(0);
            }
            // father process
            else{
                pid=x;
                signal(SIGTSTP,sigTSTP_handler);    
                pause();
                return;
            }
    }

    int main(){
        //those function will catch the signals any time
        signal(SIGTSTP,sigTSTP_handler);
        signal(SIGCHLD,sigCHLD_handler);

        int space_counter=0;
        int command_counter=0, commands_length=0;
        char command[510];
        char** first;

        for(;;){
            promptLine();   // print the prompt line
            fgets(command,510, stdin);  // receives a command
            
            if(command==NULL || strcmp(command, "\n")==0){} //if nothing entered start over
            
            if(strcmp(command,"bg\n")==0){  //if we want to revive the last program that pauesed
                kill(pid, SIGCONT);
                command_counter++;
                continue;
            }
            else{
                command_counter++;
                command[strlen(command)-1]= '\0' ;  // delete the \n
                commands_length+=strlen(command);      // add the length of the current commant to the sum

                for(int i=0; i<strlen(command); i++)// count the spaces in the received command
                    if(command[i]==' ')
                        space_counter++;

                space_counter+=2;   // for the first and the last words
                first= (char**)malloc(sizeof(char*)*space_counter);  // allocate memory for the array who hold the command words
                int idx=0;
                
                splitTheCommand(command, first, &idx);    // use a method to split the command into a double array
                //now first holds all the arguments received

                if(strcmp(first[0],"cd")==0){
                    printf("command not supported(yet)\n");
                }
                else if(strcmp(first[0],"done")==0){    // print a summary and exit the programm 
                    for(int i=0; i<idx; i++)   // free the arrays of the words
                        free(first[i]);
                    free(first);
                    printSummary(commands_length,command_counter);
                    exit(0);
                }
                else{   // if needs to run the command
                    execTheCommand(first);      
                } 
                for(int i=0; i<idx; i++)   // free the arrays of the words
                        free(first[i]);
                free(first);
            }
        }
        return 0;
    }