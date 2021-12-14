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
[Description]	: ������ TCP connection�� �δ� clinet
[Input]         : Nothing.
[Output]        : �����κ��� ���� �޼���  ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		socket()        //socket ����
		connect()	//������ ����
	read()          //������ �б�
		spritf()        //�����͸� ���۷� ���
		write()         //������ ����
		close()         //close socket
*/
int
main(int argc, char* argv[])
{
	int					sockfd, n;
	struct sockaddr_in	servAddr;
	MsgType				msg;

	//TCP socket ����
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr�� 0���� �ʱ�ȭ
	servAddr.sin_family = PF_INET;	//IPv4 internet protocol ���
	servAddr.sin_addr.s_addr = inet_addr(SERV_HOST_ADDR);	//SERV_HOST_ADDR�� 4byte integer(network byte order)�� ��ȯ
	//SERV_TCP_PORT : ���� ������ ������ port number
	//port number ����
		//port number�� network byte order�� ��ȯ(16bit)
	servAddr.sin_port = htons(SERV_TCP_PORT);

	//������ connection�� ����
	//servAddr : connect�� server�� socket address
	if (connect(sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("connect");
		exit(1);
	}

	msg.type = MSG_REQUEST;
	sprintf(msg.data, "This is a request from %d.", getpid());	//� ���μ���(Ŭ���̾�Ʈ)�� ��û�ߴ��� ���
	//������ ����
	if (write(sockfd, (char*)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}
	printf("Sent a request.....");

	//������ �б�
	if ((n = read(sockfd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}
	printf("Received reply: %s\n", msg.data);

	close(sockfd);	//close socket
}
