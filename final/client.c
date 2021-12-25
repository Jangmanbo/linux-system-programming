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
[Description]   : ip address�� �ƴ� host name�ε� ������ TCP connection�� �δ� clinet
[Input]
	int	argc	// argv �迭�� ũ��
	char	*argv[]	//������ ip �Ǵ� dotted decimal �Ǵ� hostname
[Output]        : �����κ��� ���� �޼���  ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		socket()        //socket ����
	inet_addr()	//dotted decimal������ network byte order�� ��ȯ
	gethostbyname()	//���������κ��� ip address�� ����
		connect()       //������ ����
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
	struct hostent* hp;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s IPaddress\n", argv[0]);
		exit(1);
	}

	//TCP socket ����
	if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	bzero((char*)&servAddr, sizeof(servAddr));
	servAddr.sin_family = PF_INET;
	//SERV_TCP_PORT : ���� ������ ������ port number
		//port number ����
		//port number�� network byte order�� ��ȯ(16bit)
	servAddr.sin_port = htons(SERV_TCP_PORT);

	//ip address ����
	if (isdigit(argv[1][0])) {//ip address�� ù���ڰ� ���ڸ� dotted decimal
		servAddr.sin_addr.s_addr = inet_addr(argv[1]);	//->network byte order(4byte)�� convert
	}
	else {	//host name�� ���
		//Ȩ�������� ������ �� DNS���񽺰� �ʿ�
		//���������κ��� ip address�� ����
		if ((hp = gethostbyname(argv[1])) == NULL) {
			fprintf(stderr, "Unknown host: %s\n", argv[1]);
			exit(1);
		}
		//hp->h_addr:������ ��ǥ ip address
		//IPv4�̹Ƿ� hp->h_length=4
		memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);
	}
	//hp->h_addr:������ ��ǥ ip address
	//IPv4�̹Ƿ� hp->h_length=4
	memcpy(&servAddr.sin_addr, hp->h_addr, hp->h_length);

	//������ connection�� ����
		//servAddr : connect�� server�� socket address
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
	//������ ����
	if (write(sockfd, (char*)&msg, sizeof(msg)) < 0) {
		perror("write");
		exit(1);
	}

	//������ �б�
	if ((n = read(sockfd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}
	//�����κ��� ���� �޼��� ���
	printf("%s\n", msg.data);

	close(sockfd);	//close socket
}
