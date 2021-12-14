#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp.h"

/*
[Program Name]  : TCP client program
[Description]	: 서버와 TCP connection을 맺는 clinet
[Input]         : Nothing.
[Output]        : 서버로부터 받은 메세지  출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		socket()        //socket 생성
		connect()	//서버와 연결
	read()          //데이터 읽기
		spritf()        //데이터를 버퍼로 출력
		write()         //데이터 저장
		close()         //close socket
*/
int
main(int argc, char* argv[])
{
	int					sockfd, n;
	struct sockaddr_in	servAddr;
	MsgType				msg;

	//TCP socket 생성
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr을 0으로 초기화
	servAddr.sin_family = PF_INET;	//IPv4 internet protocol 사용
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);	//SERV_HOST_ADDR을 4byte integer(network byte order)로 변환
	//SERV_TCP_PORT : 내가 접속할 서버의 port number
	//port number 지정
		//port number를 network byte order로 변환(16bit)
	servAddr.sin_port = htons(SERV_TCP_PORT);

	//서버와 connection을 맺음
	//servAddr : connect할 server의 socket address
	if (connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());	//어떤 프로세스(클라이언트)가 요청했는지 출력
	//데이터 저장
	if (write(sockfd, (char*)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");

	//데이터 읽기
	if ((n = read(sockfd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);	//close socket
}
