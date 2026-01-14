#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

int  main()
{
    int sockfd, newsockfd;
    struct sockaddr_in serv_addr;
    int i,n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // create a tcp socket

    if (sockfd < 0){
        perror("socket failed");
        exit(1);
    }

    serv_addr.sin_family = AF_INET; // specifies IPv4 addressing
    serv_addr.sin_addr.s_addr = INADDR_ANY; // server can accept connections from any IP address
    serv_addr.sin_port = htons(8080); // sets port no to 8080. htons() converts port into network byte order

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) > 0) // bind socket, ip address, port number
    {
        perror("bind error");
        exit(1);
    }

    listen(sockfd, 5); // puts server in listening mode with a maximum of 5 connection in waiting queue
    printf("Server listening on port 8080\n");

    newsockfd = accept(sockfd, NULL, NULL);// waits for client to client and returns a new socket descriptor
    
    if(newsockfd < 0){
        perror("accept");
        exit(1);
    }
    
    // communication now happens through newsockfd
    printf("Client connected\n");

    printf("Enter the limit: ");
    scanf("%d", &n);

    for(i=1;i<=n;i++){
        write(newsockfd, &i, sizeof(i));
    }

    close(newsockfd);
    close(sockfd);


    return 0;
}
