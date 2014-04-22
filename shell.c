#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include <stdio.h>

#define MAXINPUT (1025)
#define NUM_ARGS (50)
#define MAX_LEN_ARG (500)

typedef enum { false, true } bool;

extern char** environ;
char prompt[15] = "zoolander# ";
char timeout_exp_msg[25] = "Time's up. you lose.\n";
char beat_timeout_msg[35] = "Well, finished before timer...\n";
char readin_err_msg[85] = "The shell doesn't like your input. Make sure your input is less than 1024 bytes.\n";
bool TIMEOUT_LIMIT = false;


void catch_alarm(int signum)
{
    write(STDOUT_FILENO, timeout_exp_msg, sizeof(timeout_exp_msg));
    _exit(0);
}


int main(int argc, char **argv)
{
    char* usrCmd[NUM_ARGS];
    char tmp[MAX_LEN_ARG];
    char c[1];
    int timeout, inputCharCount, cmdIndex, arglen;

    signal(SIGALRM, catch_alarm);

    // Check for Command Line Arguments
    if (argc > 1){
        timeout = atoi(argv[1]);
        TIMEOUT_LIMIT = true;
    }


    while(1){
    PROMPT_USER:
        write(STDOUT_FILENO, prompt, sizeof(prompt));
        inputCharCount = 0;
        cmdIndex = 0;
        arglen = 0;

        // Read and Parse user's input from standard in
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

        // int i;
        // for(i=0; i<=cmdIndex; i++){
        //     printf("%s\n", usrCmd[i]);
        // }fflush(stdout);

        // Start timer
        if (TIMEOUT_LIMIT == true){ alarm(timeout); }  
        
        // Fork a new process
        int pid = fork();
        if (pid < 0){continue;} //TODO provide some error msg here TODO
        if (!pid){//Child
            execve(usrCmd[0], usrCmd, environ);
        }else{//Parent
            wait();
            if (TIMEOUT_LIMIT == true){
                alarm(0); 
                write(STDOUT_FILENO, beat_timeout_msg, sizeof(beat_timeout_msg));
            }  
        }
    }
}