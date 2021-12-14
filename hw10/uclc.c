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
[Description]   : ������ �����͸� �ְ���� clinet
[Input]         : Nothing.
[Output]        : �����κ��� ���� �޼���  ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		socket()        //socket ����
	bind()		//socket address ����
		recvfrom()      //client���� packet�� �ޱ⸦ ��ٸ�
		sendto()        //packet�� �޾Ҵ� client���� ������ ����
	spritf()       	//�����͸� ���۷� ���
		remove()	//���� ����
	close()         //close socket
*/
int
main(int argc, char* argv[])
{
	int					sockfd, n, servAddrLen, myAddrLen, peerAddrLen;
	struct sockaddr_un	servAddr, myAddr, peerAddr;
	MsgType				msg;

	//socket ����
	if ((sockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&myAddr, sizeof(myAddr));	//myAddr�� 0���� �ʱ�ȭ
	myAddr.sun_family = PF_UNIX;
	sprintf(myAddr.sun_path, ".unix-%d", getpid());
	myAddrLen = strlen(myAddr.sun_path) + sizeof(myAddr.sun_family);

	//socket address ����
	if (bind(sockfd, (struct sockaddr*)&myAddr, myAddrLen) < 0) {
		perror("bind");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr�� 0���� �ʱ�ȭ
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_DG_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());
	//�������� �������� ������ ����
	if (sendto(sockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&servAddr, servAddrLen) < 0) {
		perror("sendto");
		exit(1);
	}
	printf("Sent a request.....");

	peerAddrLen = sizeof(peerAddr);
	//�������κ��� ������ ����
	if ((n = recvfrom(sockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&peerAddr, &peerAddrLen)) < 0) {
		perror("recvfrom");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);	//close socket
	//���� ����
	if (remove(myAddr.sun_path) < 0) {
		perror("remove");
		exit(1);
	}
}
