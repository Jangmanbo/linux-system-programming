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
	close(Sockfd);	//close socket
	if (remove(UNIX_DG_PATH) < 0) {
		perror("remove");
	}

	printf("\nUNIX-domain Connection-Less Server exit.....\n");

	exit(0);	//���α׷� ����
}

/*
[Program Name]  : unix connection less Server program
[Description]   : client�� �����͸� �ְ���� server
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
	int					servAddrLen, cliAddrLen, n;
	struct sockaddr_un	cliAddr, servAddr;
	MsgType				msg;

	signal(SIGINT, CloseServer);

	//socket ����
	if ((Sockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr�� 0���� �ʱ�ȭ
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_DG_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	//socket address ����
	if (bind(Sockfd, (struct sockaddr*)&servAddr, servAddrLen) < 0) {
		perror("bind");
		exit(1);
	}

	printf("UNIX-domain Connection-Less Server started.....\n");

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
