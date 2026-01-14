#include <stdio.h>
#include <sys/ipc.h> //key_t for IPC keys
#include <sys/shm.h> // shmget(), shmat(), shmdt()
#include <stdlib.h> // exit()

int main(){
    key_t key; // unique shared memory key that both reader and writer use to access and create shared memory
    int shmid; // shared memory ID
    int *shm; // pointer to access shared memory like an array
    int i,n;

    key = ftok(".", 90); // "." -> current directory & 90 -> Project identifier
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    // create memory if it doesn't exist

    if(shmid == -1){
        perror("shmget error");
        exit(1);
    }

    shm = (int *)shmat(shmid, NULL, 0); // returns pointer to the shared memory

    if(shm == (int *)-1){
        perror("shmat error");
        exit(1);
    }

    printf("Shared memory created and attached\n");

    printf("Enter the limit: ");
    scanf("%d", &n);

    shm[0] = n; // used by reader to know how many numbers to read

    for (i=1; i<=n; i++){
        shm[i] = i; // each number is written directly to shared memory
        printf("Writer stored: %d\n", i);
    }

    shmdt(shm); //detach shared memory;
    printf("Writer detached from shared memory\n");

    printf("Press Enter after reader finishes...");
    getchar(); getchar(); // wait for reader

    shmctl(shmid, IPC_RMID, NULL);
    printf("Shared memory destroyed by writer\n");


    return 0;
}