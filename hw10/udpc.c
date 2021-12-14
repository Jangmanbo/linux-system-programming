#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "udp.h"


/*
[Program Name]  : UDP client program
[Description]   : 서버와 UDP connection을 맺는 clinet
[Input]         : Nothing.
[Output]        : 서버로부터 받은 메세지  출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		socket()        //socket 생성
		recvfrom()      //client에게 packet을 받기를 기다림
		sendto()        //packet을 받았던 client에게 데이터 전송
	spritf()        //데이터를 버퍼로 출력
		close()         //close socket
*/
int
main(int argc, char* argv[])
{
	int					sockfd, n, peerAddrLen;
	struct sockaddr_in	servAddr, peerAddr;
	MsgType				msg;

	//socket 생성
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr을 0으로 초기화
	servAddr.sin_family = PF_INET;			//IPv4 internel protocol 사용
	//INADDR_ANY : 어떤 ip로 오든 받음
		//ip address 지정
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	//port number 지정
		//port number를 network byte order로 변환(16bit)
	servAddr.sin_port = htons(SERV_UDP_PORT);

	msg.type = MSG_REQUEST;
	//버퍼에 데이터 저장
	sprintf(msg.data, "This is a request from %d.", getpid());
	//서버에게 소켓으로 데이터 전송
	if (sendto(sockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("sendto");
		exit(1);
	}
	printf("Sent a request.....");

	peerAddrLen = sizeof(peerAddr);
	//소켓으로부터 데이터 수신
	if ((n = recvfrom(sockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&peerAddr, &peerAddrLen)) < 0) {
		perror("recvfrom");
		exit(1);
	}
	//서버로부터 전송받은 메세지 출력
	printf("Received reply: %s\n", msg.data);

	close(sockfd);	//close socket
}
