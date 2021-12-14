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


int	Sockfd;	//signal handler���� ����ϱ� ���� ���������� ����

/*
[Function Name] : CloseServer
[Description]   : ctrl+C�� ������ ��(SIG_INT) ȣ��Ǵ� signal handler
[Input]         : Nothing.
[Output]        : ������ ����Ǿ��ٴ� �޼��� ���
[Call By]       : kernel
[Calls]
		printf()        //�޼��� ���
		close()         //socket ����
		exit()          //���α׷� ����
[Given]
	int     Sockfd  //close�� ����
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
[Description]   : client�� UDP connection�� ���� server
[Input]         : Nothing.
[Output]        : client�κ��� ���޵� �޼��� ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		signal()	//signal handler ���
	socket()        //socket ����
		bind()          //socket address ����
		recvfrom()	//client���� packet�� �ޱ⸦ ��ٸ�
	sendto()	//packet�� �޾Ҵ� client���� ������ ����
	spritf()        //�����͸� ���۷� ���
		close()         //close socket
*/
int
main(int argc, char* argv[])
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;
	MsgType				msg;

	//SIGINT handler ���
	signal(SIGINT, CloseServer);

	//UDP socket ����
	if ((Sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr�� 0���� �ʱ�ȭ
	servAddr.sin_family = PF_INET;			//IPv4 internet protocol ���
	//INADDR_ANY : � ip�� ���� ����
		//ip address ����
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//port number ����
		//port number�� network byte order�� ��ȯ(16bit)
	servAddr.sin_port = htons(SERV_UDP_PORT);

	//socket address ����
	if (bind(Sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	printf("UDP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1) {
		//packet�� ���⸦ ��ٸ�
		//cliAddr : packet�� ���� clinet�� socket address
		if ((n = recvfrom(Sockfd, (char*)&msg, sizeof(msg),
			0, (struct sockaddr*)&cliAddr, &cliAddrLen)) < 0) {
			perror("recvfrom");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//�ռ� packcet�� ���´� clinet(cliAddr)���� ����
		if (sendto(Sockfd, (char*)&msg, sizeof(msg),
			0, (struct sockaddr*)&cliAddr, cliAddrLen) < 0) {
			perror("sendto");
			exit(1);
		}
		printf("Replied.\n");
	}
}
