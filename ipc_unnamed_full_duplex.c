// Demonstration of inter process communication using unnamed pipes in full duplex mode.

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(){
    int p1[2], p2[2]; // two pipes for two way comms
    pid_t pid;
    int num, i, n;

    printf("Enter the limit: ");
    scanf("%d", &n);

    if (pipe(p1) == -1 || pipe(p2) == -1){
        perror("pipe failed");
        exit(1);
    }

    pid = fork();

    if(pid<0){
        perror("fork failed");
        exit(1);
    }

    if (pid == 0){
        // child process
        printf("Child process \n");
        close(p1[1]);
        close(p2[0]);

        while (read(p1[0], &num, sizeof(num)) > 0){
            if (num % 2 != 0){
                write(p2[1], &num, sizeof(num)); // write odd numbers into second pipe
            }
        }
        close(p1[0]);
        close(p2[1]);
        printf("child process ended\n");
    } else {
        // parent process if pid > 0
        printf("Parent process\n");
        close(p1[0]);
        close(p2[1]);

        for(i=1; i<=n; i++){
            write(p1[1], &i, sizeof(i));
        }
        close(p1[1]); // close write end of pipe 1 after writing n natural numbers

        printf("Odd numbers: \n");
        while(read(p2[0], &num, sizeof(num)) > 0) {
            printf("%d   ", num);
        }
        printf("\n");
        close(p2[0]); //close read end after printing odd numbers
        printf("Parent process ended\n");
    }
    return 0;
}