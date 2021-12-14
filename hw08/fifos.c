#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "fifo.h"


/*
[Function Name] : SigIntHandler
[Description]
	SIGINT signal�� �߻��ϸ� ȣ��Ǵ� �Լ�
	fifo file�� �����ϰ� ���μ����� �����Ŵ
[Input]         : signo	//������ signal
[Output]        : Nothing.
[Call By]	: kernel
[Calls]
	perror()	//���� �޼��� ���
	exit()		//���α׷� ����
	remove()	//file remove
[Given]		: Nothing.
[Returns]	: Nothing.
*/
void
SigIntHandler(int signo)
{
	//remove fifo file
	if (remove(SERV_FIFO) < 0) {
		perror("remove");
		exit(1);
	}

	exit(0);
}

/*
[Program Name]  : fifo server program
[Description]   : fifo�� client�κ��� ��û�ް� �����ϴ� server program
[Input]		: Nothing.
[Output]	: � client�κ��� ��û�޾Ҵ��� ���
[Calls]         :
	signal()	//signal handler ���
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	mkfifo()	//fifo file ����
	printf()	//�޼��� ���
	close()		//file descriptor close
	open()		//file open, return file descriptor
	read()		//file���� �����͸� read
	sprintf()	//���ۿ� ���ڿ� ����
	write()		//file�� ������ write
*/
int
main()
{
	int			fd, cfd, n;
	MsgType		msg;

	//SIGINT signal handler ���
	if (signal(SIGINT, SigIntHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	//fifo.h�� ���ǵ� SERV_FIFO �̸����� fifo ����
	//0600 : ���� �а� ����
	if (mkfifo(SERV_FIFO, 0600) < 0) {
		//�̹� �ش� �̸��� fifo�� �ִ� ���� ������ �ƴ�
		if (errno != EEXIST) {//�����ϸ� ���μ��� ����
			perror("mkfifo");
			exit(1);
		}
	}

	//fifo file open
	if ((fd = open(SERV_FIFO, O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	while (1) {
		//client�κ��� �޼����� ��ٸ�
		if ((n = read(fd, (char*)&msg, sizeof(msg))) < 0) {
			if (errno == EINTR) {//�ý��� �� ���� �� interrupt�� �ɷ� �ߴܵ� ���
				continue;
			}
			else {
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s.....", msg.data);

		//clinet���� �����ϱ� ���� fifo file open
		if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0) {
			perror("open");
			exit(1);
		}
		//msg.data�� ���ڿ� ����
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//fifo�� msg�� �ۼ������μ� clinet���� �޼��� ����
		write(cfd, (char*)&msg, sizeof(msg));
		//fifo close
		close(cfd);
		printf("Replied.\n");
	}
}
