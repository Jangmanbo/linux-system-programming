#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp.h"


int	Sockfd;	//signal handler에서 close해야 하므로 전역변수
/*
[Function Name] : CloseServer
[Description]	: ctrl+C를 눌렀을 때(SIG_INT) 호출되는 signal handler
[Input]		: Nothing.
[Output]        : 서버가 종료되었다는 메세지 출력
[Call By]       : kernel
[Calls]
	printf()	//메세지 출력
	close()		//socket 종료
	exit()		//프로그램 종료
[Given]
	int	Sockfd	//close할 소켓
[Returns]       : Nothing.
*/
void
CloseServer()
{
	close(Sockfd);	//socket 종료
	printf("\nTCP Server exit.....\n");

	exit(0);	//프로그램 종료
}

/*
[Program Name]  : TCP Server program
[Description]	: client와 TCP connection을 맺을 server
[Input]         : Nothing.
[Output]        : client로부터 전달된 메세지 출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
	usleep()	//n us간 대기
	socket()	//socket 생성
	bind()		//socket address 지정
	listen()	//동시 접속하는 connection을 기다릴지 지정
	accept()	//TCP connection이 맺어지기를 기다림
	read()		//데이터 읽기
	spritf()	//데이터를 버퍼로 출력
	write()		//데이터 저장
	close()		//close socket
*/
int
main(int argc, char* argv[])
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;	//IPv4
	MsgType				msg;

	signal(SIGINT, CloseServer);	//SIGINT signal handler 등록

	//TCP socket 생성
	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	//servAddr를 0으로 초기화
	bzero((char*)&servAddr, sizeof(servAddr));
	//IPv4 internet protocol 사용
	servAddr.sin_family = PF_INET;
	//INADDR_ANY : 어떤 ip로 오든 받음
	//ip address 지정
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//port number 지정
	//port number를 network byte order로 변환(16bit)
	servAddr.sin_port = htons(SERV_TCP_PORT);

	//socket address 지정(socket에 port nunber 부여)
	if (bind(Sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	//5개의 connection까지 기다림
	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1) {
		//TCP connection이 맺어지기를 기다림
		//cliAddr : connection이 맺어진 client의 socket address
		newSockfd = accept(Sockfd, (struct sockaddr*)&cliAddr, &cliAddrLen);
		if (newSockfd < 0) {
			perror("accept");
			exit(1);
		}

		//데이터 읽기
		if ((n = read(newSockfd, (char*)&msg, sizeof(msg))) < 0) {
			perror("read");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//데이터 저장
		if (write(newSockfd, (char*)&msg, sizeof(msg)) < 0) {
			perror("write");
			exit(1);
		}
		printf("Replied.\n");

		usleep(10000);	//10000us동안 sleep
		close(newSockfd);	//close socket->loop하면서 새로운 connection이 맺어지기를 또 기다림
	}
}
