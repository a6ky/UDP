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
#define CNT_LISTEN  16
#define MSG_MAXLEN  256

int main()
{
	int                sockfd;
	struct sockaddr_in servaddr;
	char               msg[MSG_MAXLEN];
	socklen_t          addr_size; 
	int                msg_size;

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sockfd == -1) {
		perror("socket");
		exit(-1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SRV_ADDR);
	servaddr.sin_port = htons(SRV_PORT);

	while (1) {
		scanf("%s", msg);
		printf("send message to server: %s\n", msg);

		addr_size = sizeof(servaddr);

		sendto(sockfd, msg, strlen(msg) + 1, 0, (struct sockaddr *)&servaddr, addr_size);

		msg_size = recvfrom(sockfd, msg, MSG_MAXLEN, 0, (struct sockaddr *)&servaddr, &addr_size);
		if (msg_size == -1) {
			perror("recvfrom");
			exit(-1);
		} else
			printf("recv message from server: %s\n", msg);
	}

	close(sockfd);
	exit(0);
}
