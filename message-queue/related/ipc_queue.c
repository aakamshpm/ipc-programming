//Demonstration of inter process communication among related processes using message queue. (full duplex)

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

struct message {
    long msgtype; // used to categorize messages in queue, receiver can request specific type
    int num; // actual data (number)
};

int main(){
    key_t key;
    int msgid;
    pid_t pid;
    int i, n;

    struct message msg; // msg object used for sending and receiving data

    key = ftok(".", 65); // unique identifier used in both parent and child

    msgid = msgget(key, 0666 | IPC_CREAT); // create queue if it doesn't exist

    if(msgid == -1){
        perror("msgid error");
        exit(1);
    }

    printf("Message Queue created: ID %d\n", msgid);

    printf("Enter limit: ");
    scanf("%d", &n);

    pid = fork();

    if(pid<0){
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) { //child
        for (i=1; i<=n; i++){
            msgrcv(msgid, &msg, sizeof(msg.num), 1, 0);
            printf("Child received: %d\n", msg.num);

            if (msg.num % 2 != 0){
                msg.msgtype = 2;
                msgsnd(msgid, &msg, sizeof(msg.num), 0);
                printf("Child sent odd number: %d\n",msg.num);
            }
        }
        exit(0);
    }

    else {
        for (i=1; i<=n; i++){
            msg.msgtype = 1;
            msg.num = i;

            msgsnd(msgid, &msg, sizeof(msg.num), 0);
            printf("Parent sent: %d\n", i);
        }

        printf("Parent receiving odd numbers:\n");
        for (i=1; i<=n; i++){
            if(msgrcv(msgid, &msg, sizeof(msg.num), 2, IPC_NOWAIT) > 0); // IPC_NOWAIT prevents parent from getting stuck in waiting
            {
                printf("Received odd number: %d\n", msg.num);
            }

        }
        msgctl(msgid, IPC_RMID, NULL);
        printf("Message Queue destryoyed \n");
    }

    return 0;
}