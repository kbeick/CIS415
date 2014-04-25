#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include <stdio.h>

#define MAXINPUT (1024)
#define MAX_NUM_ARGS (50)
#define MAX_LEN_ARG (500)

typedef enum { false, true } bool;

extern char** environ;
char prompt[15] = "arrogant# ";
char timeout_exp_msg[40] = "Time's up, you lose, I'm the best.\n";
char beat_timeout_msg[50] = "Well aren't you lucky, finished before timer...\n";
char readin_err_msg[125] = "The shell doesn't like your input. Make sure your input has at most 1024 bytes and 50 whitespace-separated tokens.\n";
bool TIMEOUT_LIMIT = false;
bool KILLED = false;
int pid;


void catch_alarm(int signum)
{
    if (pid > 0) { 
        if (kill(pid, SIGKILL) < 0) {
            perror("The following error occured while calling kill()"); 
        }   
    }
    KILLED = true;
    if ( write(STDOUT_FILENO, timeout_exp_msg, sizeof(timeout_exp_msg)) < 0) {
        perror("The following error occured while calling write()"); 
    }
}


int main(int argc, char **argv)
{
    char* usrCmd[MAX_NUM_ARGS+1];
    char tmp[MAX_NUM_ARGS+1][MAX_LEN_ARG+1];
    char c[1];
    int timeout, inputCharCount, cmdIndex, arglen, numargs;

    // SIGNAL HANDLER
    if (signal(SIGALRM, catch_alarm) < 0) {
        perror("The following error occured while calling signal()"); 
    }

    // Check for Command Line Arguments
    if (argc > 1){
        timeout = atoi(argv[1]);
        TIMEOUT_LIMIT = true;
    }


    while(1){
    PROMPT_USER:
        KILLED = false;
        if (write(STDOUT_FILENO, prompt, sizeof(prompt)) < 0){
            perror("The following error occured while calling write()"); 
        }
        inputCharCount = 0;
        cmdIndex = 0;
        arglen = 0;
        numargs = 1;

        // Read and Parse user's input from standard in
        while(read(STDIN_FILENO, c, 1) > 0){
            if (inputCharCount > MAXINPUT || numargs > MAX_NUM_ARGS){
                if (write(STDOUT_FILENO, readin_err_msg, sizeof(readin_err_msg)) < 0) {
                    perror("The following error occured while calling write()"); 
                }
                goto PROMPT_USER;
            }
            if( c[0] == '\n'){
            // End of command
                tmp[cmdIndex][arglen] = '\0';
                usrCmd[cmdIndex] = tmp[cmdIndex];
                // printf("old entry:%s\n", usrCmd[cmdIndex-2]);
                usrCmd[cmdIndex+1] = NULL;
                break;
            }else if (c[0] == ' ' || c[0] == '\t'){
            // tokenize command
                tmp[cmdIndex][arglen] = '\0';
                usrCmd[cmdIndex] = tmp[cmdIndex];
                cmdIndex++;
                numargs++;
                arglen = 0;
            }else{
            // normal character
                tmp[cmdIndex][arglen] = c[0]; 
                arglen++;
            }
            inputCharCount++;
        }

        // int i=0;
        // while(usrCmd[i] != NULL){
        //     printf("%s, %i\n", usrCmd[i], i);
        //     i++;
        // }fflush(stdout);
 
        
        // Fork a new process
        pid = fork();
        if (pid < 0){
            perror("The following error occured while calling fork()"); 
            continue;
        }
        if (!pid){//Child
            if (execve(usrCmd[0], usrCmd, environ) < 0) {
                perror("The following error occured while calling execve()"); 
            }
            _exit(0);
        }else{//Parent
            // Start timer
            if (TIMEOUT_LIMIT == true){ 
                if (alarm(timeout) < 0) {
                    perror("The following error occured while calling alarm()"); 
                }   
            } 
            if (wait(NULL) < 0) {
                perror("The following error occured while calling wait()"); 
            }
            // Done waiting, child process must have ended.
            if ( TIMEOUT_LIMIT == true && KILLED == false){
                if (alarm(0) < 0) {
                    perror("The following error occured while calling alarm()"); 
                }    
                if ( write(STDOUT_FILENO, beat_timeout_msg, sizeof(beat_timeout_msg)) < 0) {
                    perror("The following error occured while calling write()"); 
                } 
            }  
        }
    }
    printf("out of wihle loop???\n");
    fflush(stdout);
}