#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

int main(){
    int fd[2];
    pid_t pid;
    int n, num, i;

    printf("Enter the limit: ");
    scanf("%d", &n);

    if(pipe(fd) == -1){
        perror("pipe error");
        exit(1);
    }

    pid = fork();

    if(pid < 0){
        perror("fork failed");
        exit(1);
    }

    // child process
    else if(pid == 0){
        close(fd[1]); //close write end

        printf("Odd numbers: \n");

        while(read(fd[0], &num, sizeof(num)) > 0){
            if(num % 2 != 0 ){
                printf("%d  ", num);
            }

        }
        printf("\n");
        close(fd[0]);
    }

    // parent process
    else {
        close(fd[0]); //close read end

        for(i=1; i<=n; i++){
            write(fd[1], &i, sizeof(i));
        }

        close(fd[1]); // close write end after writing data into pipe
    }

    return 0;

}