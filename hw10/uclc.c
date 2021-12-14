#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "unix.h"

/*
[Program Name]  : UNIX connection less client program
[Description]   : 서버와 데이터를 주고받을 clinet
[Input]         : Nothing.
[Output]        : 서버로부터 받은 메세지  출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		socket()        //socket 생성
	bind()		//socket address 지정
		recvfrom()      //client에게 packet을 받기를 기다림
		sendto()        //packet을 받았던 client에게 데이터 전송
	spritf()       	//데이터를 버퍼로 출력
		remove()	//파일 삭제
	close()         //close socket
*/
int
main(int argc, char* argv[])
{
	int					sockfd, n, servAddrLen, myAddrLen, peerAddrLen;
	struct sockaddr_un	servAddr, myAddr, peerAddr;
	MsgType				msg;

	//socket 생성
	if ((sockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&myAddr, sizeof(myAddr));	//myAddr을 0으로 초기화
	myAddr.sun_family = PF_UNIX;
	sprintf(myAddr.sun_path, ".unix-%d", getpid());
	myAddrLen = strlen(myAddr.sun_path) + sizeof(myAddr.sun_family);

	//socket address 지정
	if (bind(sockfd, (struct sockaddr*)&myAddr, myAddrLen) < 0) {
		perror("bind");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr을 0으로 초기화
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_DG_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	//서버에게 소켓으로 데이터 전송
	if (sendto(sockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&servAddr, servAddrLen) < 0) {
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
	printf("Received reply: %s\n", msg.data);

	close(sockfd);	//close socket
	//파일 삭제
	if (remove(myAddr.sun_path) < 0) {
		perror("remove");
		exit(1);
	}
}
