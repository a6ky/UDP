#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SRV_PORT    1666
#define SRV_ADDR    "127.0.0.1"
#define MSG_MAXLEN  256

void new_client(struct sockaddr_in cliaddr, char *buf);

int main()
{
	int                sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t          addr_size; 
    char               buf[MSG_MAXLEN];
    int                msg_size;
    pid_t              pid;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1){
        perror("socket");
        exit(-1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(SRV_ADDR);
    servaddr.sin_port = htons(SRV_PORT);

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        exit(-1);
    }

    addr_size = sizeof(cliaddr);

    while (1) {
        msg_size = recvfrom(sockfd, buf, MSG_MAXLEN, 0, (struct sockaddr *)&cliaddr, &addr_size);
        if (msg_size == 0)
            break;
        if (msg_size == -1) {
            perror("recvfrom");
            exit(-1);
        } else
            printf("recive message from client: %s\n", buf);

        if ((pid = fork()) == 0)
            new_client(cliaddr, buf);
    }

    close(sockfd);
    exit(0);
}

void new_client(struct sockaddr_in cliaddr, char *buf) 
{   
    int                sockfd;
    struct sockaddr_in servaddr;
    socklen_t          addr_size; 
    int                msg_size;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1) {
        perror("socket");
        exit(-1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, SRV_ADDR, &(servaddr.sin_addr));  // преобразуем SRV_ADDR в сетевой порядок байт
    servaddr.sin_port = 0;

    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        perror("bind");
        exit(-1);
    }

    addr_size = sizeof(cliaddr);

    printf("port number %d\n", ntohs(servaddr.sin_port));

    while (1) {
        printf("send message to client: %s\n", buf);
        sendto(sockfd, buf, strlen(buf) + 1, 0, (struct sockaddr *)&cliaddr, addr_size);
        msg_size = recvfrom(sockfd, buf, MSG_MAXLEN, 0, (struct sockaddr *)&cliaddr, &addr_size);
    }

    close(sockfd);
    exit(0);
}


