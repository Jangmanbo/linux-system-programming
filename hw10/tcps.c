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


int	Sockfd;	//signal handler���� close�ؾ� �ϹǷ� ��������
/*
[Function Name] : CloseServer
[Description]	: ctrl+C�� ������ ��(SIG_INT) ȣ��Ǵ� signal handler
[Input]		: Nothing.
[Output]        : ������ ����Ǿ��ٴ� �޼��� ���
[Call By]       : kernel
[Calls]
	printf()	//�޼��� ���
	close()		//socket ����
	exit()		//���α׷� ����
[Given]
	int	Sockfd	//close�� ����
[Returns]       : Nothing.
*/
void
CloseServer()
{
	close(Sockfd);	//socket ����
	printf("\nTCP Server exit.....\n");

	exit(0);	//���α׷� ����
}

/*
[Program Name]  : TCP Server program
[Description]	: client�� TCP connection�� ���� server
[Input]         : Nothing.
[Output]        : client�κ��� ���޵� �޼��� ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
	usleep()	//n us�� ���
	socket()	//socket ����
	bind()		//socket address ����
	listen()	//���� �����ϴ� connection�� ��ٸ��� ����
	accept()	//TCP connection�� �ξ����⸦ ��ٸ�
	read()		//������ �б�
	spritf()	//�����͸� ���۷� ���
	write()		//������ ����
	close()		//close socket
*/
int
main(int argc, char* argv[])
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr, servAddr;	//IPv4
	MsgType				msg;

	signal(SIGINT, CloseServer);	//SIGINT signal handler ���

	//TCP socket ����
	if ((Sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	//servAddr�� 0���� �ʱ�ȭ
	bzero((char*)&servAddr, sizeof(servAddr));
	//IPv4 internet protocol ���
	servAddr.sin_family = PF_INET;
	//INADDR_ANY : � ip�� ���� ����
	//ip address ����
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//port number ����
	//port number�� network byte order�� ��ȯ(16bit)
	servAddr.sin_port = htons(SERV_TCP_PORT);

	//socket address ����(socket�� port nunber �ο�)
	if (bind(Sockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	//5���� connection���� ��ٸ�
	listen(Sockfd, 5);

	printf("TCP Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1) {
		//TCP connection�� �ξ����⸦ ��ٸ�
		//cliAddr : connection�� �ξ��� client�� socket address
		newSockfd = accept(Sockfd, (struct sockaddr*)&cliAddr, &cliAddrLen);
		if (newSockfd < 0) {
			perror("accept");
			exit(1);
		}

		//������ �б�
		if ((n = read(newSockfd, (char*)&msg, sizeof(msg))) < 0) {
			perror("read");
			exit(1);
		}
		printf("Received request: %s.....", msg.data);

		msg.type = MSG_REPLY;
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//������ ����
		if (write(newSockfd, (char*)&msg, sizeof(msg)) < 0) {
			perror("write");
			exit(1);
		}
		printf("Replied.\n");

		usleep(10000);	//10000us���� sleep
		close(newSockfd);	//close socket->loop�ϸ鼭 ���ο� connection�� �ξ����⸦ �� ��ٸ�
	}
}
