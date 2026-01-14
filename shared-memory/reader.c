#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int main(){
    key_t key;
    int shmid;
    int *shm;
    int i,n;

    key = ftok(".", 90); // generate the same key used in writer with same properties

    shmid = shmget(key, 1024, 0666); // access shared memory using the key (no IPC_CREAT - already created memory in writer)

    if(shmid == -1){
        perror("shmid error");
        exit(1);
    }

    shm = (int *)shmat(shmid, NULL, 0); // reader attached to memory

    if(shm == (int *)-1){
        perror("shmat error");
        exit(1);
    }

    printf("Reader attached to Shared memory\n");

    printf("Odd Numbers: \n");

    n = shm[0];

    for (i=1; i<=n; i++){
        if(shm[i] % 2 != 0){
            printf("%d   ", shm[i]);
        }
    }
    printf("\n");
    shmdt(shm);

    printf("Shared Memory detached\n");

    return 0;
}