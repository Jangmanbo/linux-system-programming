#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "unix.h"


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
	close(Sockfd);	//socket close
	if (remove(UNIX_STR_PATH) < 0) {
		perror("remove");
	}

	printf("\nUNIX-domain Connection-Oriented Server exit.....\n");

	exit(0);	//���α׷� ����
}


/*
[Program Name]  : UNIX domain connection oriented Server program
[Description]	: client�� �����͸� �ְ���� server
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
	int					newSockfd, servAddrLen, cliAddrLen, n;
	struct sockaddr_un	cliAddr, servAddr;
	MsgType				msg;

	signal(SIGINT, CloseServer);

	//socket ����
	if ((Sockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr�� 0���� �ʱ�ȭ
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_STR_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	//socket address ����
	if (bind(Sockfd, (struct sockaddr*)&servAddr, servAddrLen) < 0) {
		perror("bind");
		exit(1);
	}

	listen(Sockfd, 5);

	printf("UNIX-domain Connection-Oriented Server started.....\n");

	cliAddrLen = sizeof(cliAddr);
	while (1) {
		//connection�� �ξ��� ������ ��ٸ�
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

		close(newSockfd);	//close socket
	}
}
