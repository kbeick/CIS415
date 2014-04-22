#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include <stdio.h>

#define MAXINPUT (1025)



int main()
{
    char prompt[10] = "\nkevin# ";
    char readin_err_msg[85] = "\nThe shell doesn't like your input. Make sure your input is less than 1024 bytes.";
    char input[MAXINPUT];
    int nbyts_read;

    while(1){
        write(STDOUT_FILENO, prompt, sizeof(prompt));
        input[0] = '\0'; // "Clear" the string
        input[MAXINPUT-1] = '\0'; // "Clear" the string

        // Read from standard in
        nbyts_read = read(STDIN_FILENO, input, MAXINPUT);
        printf("the number of bytes read: %d", nbyts_read);
        fflush(stdout);
        if (nbyts_read < 0){
            write(STDOUT_FILENO, readin_err_msg, sizeof(readin_err_msg));
            continue;
        }
        else if(input[ MAXINPUT - 1 ] != '\n' && input[ MAXINPUT - 1 ] != '\0') {
            write(STDOUT_FILENO, readin_err_msg, sizeof(readin_err_msg));
            continue;
        }
        

        // pid = fork();
        // if (!pid){
        //     execve(cmd, ...);
        // }else{
        //     wait();
    }
}