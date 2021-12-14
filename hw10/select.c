#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include "select.h"


//signal handler에서 사용하기 위해 전역변수로 선언
int	TcpSockfd;
int	UdpSockfd;
int	UcoSockfd;
int	UclSockfd;

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
		int     TcpSockfd, UdpSockfd, UcoSockfd, UclSockfd	//close할 소켓의 file descriptor
[Returns]       : Nothing.
*/
void
CloseServer()
{
	//close socket
	close(TcpSockfd);
	close(UdpSockfd);
	close(UcoSockfd);
	close(UclSockfd);
	if (remove(UNIX_STR_PATH) < 0) {
		perror("remove");
	}
	if (remove(UNIX_DG_PATH) < 0) {
		perror("remove");
	}

	printf("\nServer daemon exit.....\n");

	exit(0);
}

//TCP socket 생성
void
MakeTcpSocket()
{
	struct sockaddr_in	servAddr;

	//socket 생성
	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
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
	servAddr.sin_port = htons(SERV_TCP_PORT);

	//socket address 지정(socket에 port nunber 부여)
	if (bind(TcpSockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	//5개의 connection까지 기다림
	listen(TcpSockfd, 5);
}

//UDP socket 생성
void
MakeUdpSocket()
{
	struct sockaddr_in	servAddr;

	//socket 생성
	if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
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

	//socket address 지정(socket에 port nunber 부여)
	if (bind(UdpSockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

}

//UNIX domain connection oriented socket 생성
void
MakeUcoSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	//socket 생성
	if ((UcoSockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr을 0으로 초기화
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_STR_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	//socket address 지정
	if (bind(UcoSockfd, (struct sockaddr*)&servAddr, servAddrLen) < 0) {
		perror("bind");
		exit(1);
	}

	listen(UcoSockfd, 5);

}

//UNIX domain connection less socket 생성
void
MakeUclSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	//socket 생성
	if ((UclSockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr을 0으로 초기화
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_DG_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	//socket address 지정
	if (bind(UclSockfd, (struct sockaddr*)&servAddr, servAddrLen) < 0) {
		perror("bind");
		exit(1);
	}

}

//TCP socket 처리
void
ProcessTcpRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	//TCP connection이 맺어지기를 기다림
	//cliAddr : connection이 맺어진 client의 socket address
	newSockfd = accept(TcpSockfd, (struct sockaddr*)&cliAddr, &cliAddrLen);
	if (newSockfd < 0) {
		perror("accept");
		exit(1);
	}

	//데이터 읽기
	if ((n = read(newSockfd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}
	printf("Received TCP request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	//데이터 저장
	if (write(newSockfd, (char*)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}
	printf("Replied.\n");

	close(newSockfd);	//close socket
}

//UDP socket 처리
void
ProcessUdpRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	//packet이 오기를 기다림
	//cliAddr : packet을 보낸 clinet의 socket address
	if ((n = recvfrom(UdpSockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&cliAddr, &cliAddrLen)) < 0) {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UDP request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	//앞서 packcet을 보냈던 clinet(cliAddr)에게 응답
	if (sendto(UdpSockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&cliAddr, cliAddrLen) < 0) {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
}

//UNIX domain connection oriented socket 생성
void
ProcessUcoRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	//connection이 맺어질 떄까지 기다림
	//cliAddr : connection이 맺어진 client의 socket address
	newSockfd = accept(UcoSockfd, (struct sockaddr*)&cliAddr, &cliAddrLen);
	if (newSockfd < 0) {
		perror("accept");
		exit(1);
	}

	//데이터 읽기
	if ((n = read(newSockfd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}
	printf("Received UNIX-domain CO request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	//데이터 저장
	if (write(newSockfd, (char*)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}
	printf("Replied.\n");

	close(newSockfd);
}

//UNIX domain connection less socket 생성
void
ProcessUclRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	//packet이 오기를 기다림
		//cliAddr : packet을 보낸 clinet의 socket address
	if ((n = recvfrom(UclSockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&cliAddr, &cliAddrLen)) < 0) {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UNIX-domain CL request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	//앞서 packcet을 보냈던 clinet(cliAddr)에게 응답
	if (sendto(UclSockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&cliAddr, cliAddrLen) < 0) {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
}

/*
[Program Name]  : select program
[Description]   : 여러 개의 TCP connection을 하나의 프로세스에서 처리하는 프로그램
[Input]
	int	argc	//입력한 argv배열의 크기
	char	*argv[]
[Output]        : Nothing.
[Calls]         :
		printf()        //메세지 출력
	signal()	//signal handler 등록
	MakeTcpSocket()	//TCP socket 생성
	MakeUdpSocket() //UDP socket 생성
	MakeUcoSocket() //UNIX domain connection oriented socket 생성
	MakeUclSocket() //UNIX domain connection less socket 생성
	FD_ZERO()	//0으로 초기화
	FD_SET()	//set에 fd 추가
	FD_ISSET()	//어느 socket에서 왔는지 확인
	select()	//동시에 몇 개의 socket이 도책했는지 리턴
*/
int
main(int argc, char* argv[])
{
	fd_set	fdvar;
	int		count;

	//SIGINT signal handler 등록
	signal(SIGINT, CloseServer);

	//socket 생성
	MakeTcpSocket();
	MakeUdpSocket();
	MakeUcoSocket();
	MakeUclSocket();

	printf("Server daemon started.....\n");

	while (1) {
		//0으로 초기화
		FD_ZERO(&fdvar);
		//fdvar에 socket 추가(추가한 socket들을 처리)
		FD_SET(TcpSockfd, &fdvar);
		FD_SET(UdpSockfd, &fdvar);
		FD_SET(UcoSockfd, &fdvar);
		FD_SET(UclSockfd, &fdvar);
		//count : 동시에 몇 개의 socket이 도착했는지
		if ((count = select(10, &fdvar, (fd_set*)NULL, (fd_set*)NULL,
			(struct timeval*)NULL)) < 0) {
			perror("select");
			exit(1);
		}
		//도착한 socket 모두 처리
		while (count--) {
			//socket 유형에 맞춰 처리
			if (FD_ISSET(TcpSockfd, &fdvar)) {
				ProcessTcpRequest();
			}
			else if (FD_ISSET(UdpSockfd, &fdvar)) {
				ProcessUdpRequest();
			}
			else if (FD_ISSET(UcoSockfd, &fdvar)) {
				ProcessUcoRequest();
			}
			else if (FD_ISSET(UclSockfd, &fdvar)) {
				ProcessUclRequest();
			}
		}
	}
}
