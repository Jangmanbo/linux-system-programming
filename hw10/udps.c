#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "udp.h"


int	Sockfd;	//signal handler에서 사용하기 위해 전역변수로 설정

/*
[Function Name] : CloseServer
[Description]   : ctrl+C를 눌렀을 때(SIG_INT) 호출되는 signal handler
[Input]         : Nothing.
[Output]        : 서버가 종료되었다는 메세지 출력
[Call By]       : kernel
[Calls]
		printf()        //메세지 출력
		close()         //socket 종료
		exit()          //프로그램 종료
[Given]
	int     Sockfd  //close할 소켓
[Returns]       : Nothing.
*/
void
CloseServer()
{
	close(Sockfd);
	printf("\nUDP Server exit.....\n");

	exit(0);
}

/*
[Program Name]  : UDP Server program
[Description]   : client와 UDP connection을 맺을 server
[Input]         : Nothing.
[Output]        : client로부터 전달된 메세지 출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		signal()	//signal handler 등록
	socket()        //socket 생성
		bind()          //socket address 지정
		recvfrom()	//client에게 packet을 받기를 기다림
	sendto()	//packet을 받았던 client에게 데이터 전송
	spritf()        //데이터를 버퍼로 출력
		close()         //close socket
*/
int
main(int argc, char* argv[])
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	MsgType				msg;

	//SIGINT handler 등록
	signal(SIGINT, CloseServer);

	//UDP socket 생성
	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr을 0으로 초기화
	servAddr.sin_family = PF_INET;			//IPv4 internet protocol 사용
	//INADDR_ANY : 어떤 ip로 오든 받음
		//ip address 지정
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//port number 지정
		//port number를 network byte order로 변환(16bit)
	servAddr.sin_port = htons(SERV_UDP_PORT);

	//socket address 지정
	if (bind(Sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	printf("UDP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1) {
		//packet이 오기를 기다림
		//cliAddr : packet을 보낸 clinet의 socket address
		if ((n = recvfrom(Sockfd, (char*)&msg, sizeof(msg),
			0, (struct sockaddr*)&cliAddr, &cliAddrLen)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//앞서 packcet을 보냈던 clinet(cliAddr)에게 응답
		if (sendto(Sockfd, (char*)&msg, sizeof(msg),
			0, (struct sockaddr*)&cliAddr, cliAddrLen) < 0) {
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}
