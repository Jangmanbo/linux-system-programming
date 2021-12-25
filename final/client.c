#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define	SERV_TCP_PORT	(7000 + 36)
#define	SERV_HOST_ADDR	"127.0.0.1"

#define	MSG_REQUEST		1
#define	MSG_REPLY		2

typedef struct {
	int		type;
	char	data[128];
}
MsgType;

/*
[Program Name]  : TCP client program
[Description]   : ip address가 아닌 host name로도 서버와 TCP connection을 맺는 clinet
[Input]
	int	argc	// argv 배열의 크기
	char	*argv[]	//서버의 ip 또는 dotted decimal 또는 hostname
[Output]        : 서버로부터 받은 메세지  출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		socket()        //socket 생성
	inet_addr()	//dotted decimal형식을 network byte order로 변환
	gethostbyname()	//도메인으로부터 ip address를 얻음
		connect()       //서버와 연결
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
	struct hostent* hp;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
		exit(1);
	}

	//TCP socket 생성
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	//SERV_TCP_PORT : 내가 접속할 서버의 port number
		//port number 지정
		//port number를 network byte order로 변환(16bit)
	servAddr.sin_port = htons(SERV_TCP_PORT);

	//ip address 지정
	if (isdigit(argv[1][0])) {//ip address의 첫글자가 숫자면 dotted decimal
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);	//->network byte order(4byte)로 convert
	}
	else {	//host name인 경우
		//홈페이지에 접속할 때 DNS서비스가 필요
		//도메인으로부터 ip address를 얻음
		if ((hp = gethostbyname(argv[1])) == NULL) {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		//hp->h_addr:서버의 대표 ip address
		//IPv4이므로 hp->h_length=4
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}
	//hp->h_addr:서버의 대표 ip address
	//IPv4이므로 hp->h_length=4
	memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);

	//서버와 connection을 맺음
		//servAddr : connect할 server의 socket address
	if (connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "%s", " ");
	for (int i = 2; i < argc; i++) {
		strcat(msg.data, argv[i]);
		if (i < argc - 1) {
			strcat(msg.data, " ");
		}
	}
	//데이터 저장
	if (write(sockfd, (char*)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}

	//데이터 읽기
	if ((n = read(sockfd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}
	//서버로부터 받은 메세지 출력
	printf("%s\n", msg.data);

	close(sockfd);	//close socket
}
