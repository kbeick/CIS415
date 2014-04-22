#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include <stdio.h>

#define MAXINPUT (1025)
#define NUM_ARGS (50)
#define MAX_LEN_ARG (500)

extern char** environ;



int main()
{
    char prompt[10] = "kevin# ";
    char readin_err_msg[85] = "The shell doesn't like your input. Make sure your input is less than 1024 bytes.\n";
    char* usrCmd[NUM_ARGS];
    char tmp[MAX_LEN_ARG];
    char c[1];
    int inputCharCount, cmdIndex, arglen;


    while(1){
    PROMPT_USER:
        write(STDOUT_FILENO, prompt, sizeof(prompt));
        inputCharCount = 0;
        cmdIndex = 0;
        arglen = 0;

        // Read user's input from standard in
        while(read(STDIN_FILENO, c, 1) > 0){
            if (inputCharCount > MAXINPUT){
                write(STDOUT_FILENO, readin_err_msg, sizeof(readin_err_msg));
                goto PROMPT_USER;
            }
            if( c[0] == '\n'){
            // End of command
                tmp[arglen] = '\0';
                usrCmd[cmdIndex] = tmp;
                usrCmd[cmdIndex+1] = NULL;
                break;
            }else if (c[0] == ' ' || c[0] == '\t'){
            // tokenize command
                tmp[arglen] = '\0';
                usrCmd[cmdIndex] = tmp;
                cmdIndex++;
                arglen = 0;
            }else{
            // normal character
                tmp[arglen] = c[0]; 
                arglen++;
            }
            inputCharCount++;
        }

        int i;
        for(i=0; i<=cmdIndex; i++){
            printf("%s\n", usrCmd[i]);
        }fflush(stdout);
        
        // Fork a new process
        int pid = fork();
        if (pid < 0){continue;} //TODO provide some error msg here TODO
        if (!pid){//Child
            execve(usrCmd[0], usrCmd, environ);
        }else{//Parent
            wait();
        }
    }
}