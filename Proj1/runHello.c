#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#define __NR_hello 340

long int hello_syscall(void) {
    return syscall(__NR_hello);
}

int main(int argc, char *argv[]) {
    long int output = hello_syscall();
    printf ("The hello_syscall() returned %ld\n", output);
    return 0;
}