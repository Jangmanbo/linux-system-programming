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
#include <pthread.h>
#include "select.h"


//signal handler���� ����ϱ� ���� ���������� ����
int	TcpSockfd;
int	UdpSockfd;
int	UcoSockfd;
int	UclSockfd;

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
		int     TcpSockfd, UdpSockfd, UcoSockfd, UclSockfd	//close�� ������ file descriptor
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

//TCP socket ����
void
MakeTcpSocket()
{
	struct sockaddr_in	servAddr;

	//socket ����
	if ((TcpSockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
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
	servAddr.sin_port = htons(SERV_TCP_PORT);

	//socket address ����(socket�� port nunber �ο�)
	if (bind(TcpSockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	//5���� connection���� ��ٸ�
	listen(TcpSockfd, 5);
}

//UDP socket ����
void
MakeUdpSocket()
{
	struct sockaddr_in	servAddr;

	//socket ����
	if ((UdpSockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
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

	//socket address ����(socket�� port nunber �ο�)
	if (bind(UdpSockfd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

}

//UNIX domain connection oriented socket ����
void
MakeUcoSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	//socket ����
	if ((UcoSockfd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr�� 0���� �ʱ�ȭ
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_STR_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	//socket address ����
	if (bind(UcoSockfd, (struct sockaddr*)&servAddr, servAddrLen) < 0) {
		perror("bind");
		exit(1);
	}

	listen(UcoSockfd, 5);

}

//UNIX domain connection less socket ����
void
MakeUclSocket()
{
	struct sockaddr_un	servAddr;
	int					servAddrLen;

	//socket ����
	if ((UclSockfd = socket(PF_UNIX, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));	//servAddr�� 0���� �ʱ�ȭ
	servAddr.sun_family = PF_UNIX;
	strcpy(servAddr.sun_path, UNIX_DG_PATH);
	servAddrLen = strlen(servAddr.sun_path) + sizeof(servAddr.sun_family);

	//socket address ����
	if (bind(UclSockfd, (struct sockaddr*)&servAddr, servAddrLen) < 0) {
		perror("bind");
		exit(1);
	}

}

//TCP socket ó��
void
ProcessTcpRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	//TCP connection�� �ξ����⸦ ��ٸ�
	//cliAddr : connection�� �ξ��� client�� socket address
	newSockfd = accept(TcpSockfd, (struct sockaddr*)&cliAddr, &cliAddrLen);
	if (newSockfd < 0) {
		perror("accept");
		exit(1);
	}

	//������ �б�
	if ((n = read(newSockfd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}
	printf("Received TCP request: %s.....", msg.data);

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

//UDP socket ó��
void
ProcessUdpRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_in	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	//packet�� ���⸦ ��ٸ�
	//cliAddr : packet�� ���� clinet�� socket address
	if ((n = recvfrom(UdpSockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&cliAddr, &cliAddrLen)) < 0) {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UDP request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	//�ռ� packcet�� ���´� clinet(cliAddr)���� ����
	if (sendto(UdpSockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&cliAddr, cliAddrLen) < 0) {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
}

//UNIX domain connection oriented socket ����
void
ProcessUcoRequest()
{
	int					newSockfd, cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	//connection�� �ξ��� ������ ��ٸ�
	//cliAddr : connection�� �ξ��� client�� socket address
	newSockfd = accept(UcoSockfd, (struct sockaddr*)&cliAddr, &cliAddrLen);
	if (newSockfd < 0) {
		perror("accept");
		exit(1);
	}

	//������ �б�
	if ((n = read(newSockfd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}
	printf("Received UNIX-domain CO request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	//������ ����
	if (write(newSockfd, (char*)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}
	printf("Replied.\n");

	close(newSockfd);
}

//UNIX domain connection less socket ����
void
ProcessUclRequest()
{
	int					cliAddrLen, n;
	struct sockaddr_un	cliAddr;
	MsgType				msg;

	cliAddrLen = sizeof(cliAddr);
	//packet�� ���⸦ ��ٸ�
		//cliAddr : packet�� ���� clinet�� socket address
	if ((n = recvfrom(UclSockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&cliAddr, &cliAddrLen)) < 0) {
		perror("recvfrom");
		exit(1);
	}
	printf("Received UNIX-domain CL request: %s.....", msg.data);

	msg.type = MSG_REPLY;
	sprintf(msg.data, "This is a reply from %d.", getpid());
	//�ռ� packcet�� ���´� clinet(cliAddr)���� ����
	if (sendto(UclSockfd, (char*)&msg, sizeof(msg),
		0, (struct sockaddr*)&cliAddr, cliAddrLen) < 0) {
		perror("sendto");
		exit(1);
	}
	printf("Replied.\n");
}

/*
[Program Name]  : server thread program
[Description]   : thread�� �����Ͽ� socket�� ó���ϴ� ���α׷�
[Input]
	int	argc	//�Է��� argv�迭�� ũ��
	char	*argv[]
[Output]        : Nothing.
[Calls]         :
		printf()        //�޼��� ���
	signal()	//signal handler ���
	MakeTcpSocket()	//TCP socket ����
	MakeUdpSocket() //UDP socket ����
	MakeUcoSocket() //UNIX domain connection oriented socket ����
	MakeUclSocket() //UNIX domain connection less socket ����
	pthread_create()	//thread ����
	pthread_join()		//thread�� ����Ǳ⸦ ��ٸ�
*/
int
main(int argc, char* argv[])
{
	fd_set	fdvar;
	int		count;
	pthread_t tid1, tid2, tid3, tid4;

	//SIGINT signal handler ���
	signal(SIGINT, CloseServer);

	//socket ����
	MakeTcpSocket();
	MakeUdpSocket();
	MakeUcoSocket();
	MakeUclSocket();

	printf("Server daemon started.....\n");

	while (1) {
		//�� socket�� ó���ϴ� thread ����
		if (pthread_create(&tid1, NULL, (void*)ProcessTcpRequest, (void*)NULL) < 0) {
			perror("pthread_create");
			exit(1);
		}

		if (pthread_create(&tid2, NULL, (void*)ProcessUdpRequest, (void*)NULL) < 0) {
			perror("pthread_create");
			exit(1);
		}
		if (pthread_create(&tid3, NULL, (void*)ProcessUcoRequest, (void*)NULL) < 0) {
			perror("pthread_create");
			exit(1);
		}

		if (pthread_create(&tid4, NULL, (void*)ProcessUclRequest, (void*)NULL) < 0) {
			perror("pthread_create");
			exit(1);
		}

		//4���� ������ �� �ϳ��� �����ϸ� �ٽ� loop
		if (pthread_join(tid1, NULL) == 0 || pthread_join(tid2, NULL) == 0 || pthread_join(tid3, NULL) == 0 || pthread_join(tid4, NULL) == 0) {
			continue;
		}

	}
}
