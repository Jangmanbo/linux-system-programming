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
[Description]   : ������ UDP connection�� �δ� clinet
[Input]         : Nothing.
[Output]        : �����κ��� ���� �޼���  ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		socket()        //socket ����
		recvfrom()      //client���� packet�� �ޱ⸦ ��ٸ�
		sendto()        //packet�� �޾Ҵ� client���� ������ ����
	spritf()        //�����͸� ���۷� ���
		close()         //close socket
*/
int
main(int argc, char* argv[])
{
	int					sockfd, n, peerAddrLen;
	struct sockaddr_in	servAddr, peerAddr;
	MsgType				msg;

	//socket ����
	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr�� 0���� �ʱ�ȭ
	servAddr.sin_family = PF_INET;			//IPv4 internel protocol ���
	//INADDR_ANY : � ip�� ���� ����
		//ip address ����
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);
	//port number ����
		//port number�� network byte order�� ��ȯ(16bit)
	servAddr.sin_port = htons(SERV_UDP_PORT);

	msg.type = MSG_REQUEST;
	//���ۿ� ������ ����
	sprintf(msg.data, "This is a request from %d.", getpid());
	//�������� �������� ������ ����
	if (sendto(sockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
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
	//�����κ��� ���۹��� �޼��� ���
	printf("Received reply: %s\n", msg.data);

	close(sockfd);	//close socket
}
