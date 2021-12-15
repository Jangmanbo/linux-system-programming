#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>



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
	char	data[128];

	//fifo.h�� ���ǵ� SERV_FIFO �̸����� fifo ����
	//0600 : ���� �а� ����
	if (mkfifo("./.myfifo", 0600) < 0) {
		//�̹� �ش� �̸��� fifo�� �ִ� ���� ������ �ƴ�
		if (errno != EEXIST) {//�����ϸ� ���μ��� ����
			perror("mkfifo");
			exit(1);
		}
	}

	//fifo file open
	if ((fd = open("./.myfifo", O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	while (1) {
		//client�κ��� �޼����� ��ٸ�
		if ((n = read(fd, data, sizeof(data))) < 0) {
			if (errno == EINTR) {//�ý��� �� ���� �� interrupt�� �ɷ� �ߴܵ� ���
				continue;
			}
			else {
				perror("read");
				exit(1);
			}
		}
		printf("FIFO1: Received a message: %s\n", data);
		//msg.data�� ���ڿ� ����
		sprintf(data, "Hi");
		//fifo�� msg�� �ۼ������μ� clinet���� �޼��� ����
		write(fd, data, sizeof(data));
		printf("FIFO1: Sent a message: %s\n", data);
		//fifo close
		close(fd);
		break;
	}
	//remove fifo file
	if (remove("./.myfifo") < 0) {
		perror("remove");
		exit(1);
	}

}
