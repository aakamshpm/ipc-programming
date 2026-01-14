#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int sockfd; //client socket descriptor
    struct sockaddr_in serv_addr; // stores server address details
    int num; // stores number received from server

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create a tcp socket
    if(sockfd < 0){
        perror("socket failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080); //server port no

    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // connects to localhost

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // connects with server (tcp)
    {
        perror("connection failed");
        exit(1);
    }

    printf("Connected to server\n");

    printf("odd numbers received: \n");

    while(read(sockfd, &num, sizeof(num)) > 0) {
        if (num % 2 != 0){
            printf("%d  ", num);
        }
    }
    printf("\n");

    close(sockfd);

    return 0;
}