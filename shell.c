#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include <stdio.h>

#define MAXINPUT (1025)
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
    if (pid > 0) { kill(pid, SIGKILL); }
    KILLED = true;
    // else{ printf("dammit!!!!\n");}
    // fflush(stdout);
    write(STDOUT_FILENO, timeout_exp_msg, sizeof(timeout_exp_msg));
    // _exit(0);
}


int main(int argc, char **argv)
{
    char* usrCmd[MAX_NUM_ARGS];
    char tmp[MAX_NUM_ARGS][MAX_LEN_ARG];
    char c[1];
    int timeout, inputCharCount, cmdIndex, arglen, numargs;

    // SIGNAL HANDLERS
    signal(SIGALRM, catch_alarm);

    // Check for Command Line Arguments
    if (argc > 1){
        timeout = atoi(argv[1]);
        TIMEOUT_LIMIT = true;
    }


    while(1){
    PROMPT_USER:
        KILLED = false;
        write(STDOUT_FILENO, prompt, sizeof(prompt));
        inputCharCount = 0;
        cmdIndex = 0;
        arglen = 0;
        numargs = 1;

        // Read and Parse user's input from standard in
        while(read(STDIN_FILENO, c, 1) > 0){
            if (inputCharCount > MAXINPUT || numargs > MAX_NUM_ARGS){
                write(STDOUT_FILENO, readin_err_msg, sizeof(readin_err_msg));
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
        if (pid < 0){continue;} //TODO provide some error msg here TODO
        if (!pid){//Child
            execve(usrCmd[0], usrCmd, environ);
            _exit(0);
        }else{//Parent
            // Start timer
            if (TIMEOUT_LIMIT == true){ alarm(timeout); } 
            wait(NULL);
            // Done waiting, child process must have ended.
            if ( TIMEOUT_LIMIT == true && KILLED == false){
                alarm(0); 
                write(STDOUT_FILENO, beat_timeout_msg, sizeof(beat_timeout_msg));
            }  
        }
    }
    printf("out of wihle loop???\n");
    fflush(stdout);
}