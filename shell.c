#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include <stdio.h>

#define MAXINPUT (1025)



int main()
{
    char prompt[10] = "kevin# ";
    char readin_err_msg[85] = "The shell doesn't like your input. Make sure your input is less than 1024 bytes.\n";
    char input[MAXINPUT];
    int nbyts_read;

    while(1){
        write(STDOUT_FILENO, prompt, sizeof(prompt));
        input[0] = '\0'; // "Clear" the string
        input[MAXINPUT-1] = '\0'; // "Clear" the string

        // Read user's input from standard in
        nbyts_read = read(STDIN_FILENO, input, MAXINPUT);
        // printf("the number of bytes read: %d", nbyts_read);
        fflush(stdout);
        if (nbyts_read < 0){
            write(STDOUT_FILENO, readin_err_msg, sizeof(readin_err_msg));
            continue;
        }
        else if(input[ MAXINPUT - 1 ] != '\n' && input[ MAXINPUT - 1 ] != '\0') {
            write(STDOUT_FILENO, readin_err_msg, sizeof(readin_err_msg));
            continue;
        }

        // Tokenize the user's input
        char* usrCmd[50];
        int cmdIndex, newTokInd, i = 0;
        for(i=0; i<MAXINPUT; i++){
            if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n'){
                usrCmd[cmdIndex] = &input[newTokInd];
                // usrCmd[cmdIndex][i - newTokInd] = "\0";
                newTokInd = i+1;
                cmdIndex++;
            }else if (input[i] == '\0'){break;}
        }
        usrCmd[cmdIndex] = NULL;

        for(i=0; i<=cmdIndex; i++){
            printf("%s\n", usrCmd[i]);
        }fflush(stdout);
        
        // // Fork a new process
        // pid = fork();
        // if (pid < 0){continue;} //TODO provide some error msg here TODO
        // if (!pid){//Child
        //     execve(cmd, ...);
        // }else{//Parent
        //     wait();
        // }
    }
}