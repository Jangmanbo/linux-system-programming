#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


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
	FILE* ffd;

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

		char* ptr = strtok(data, " ");      // " " ���� ���ڸ� �������� ���ڿ��� �ڸ�, ������ ��ȯ
		char fname[128];
		sprintf(fname, ptr);
		strcat(fname, ".txt");
		if ((ffd = fopen(fname, "at")) == NULL) {//argv[2] text file open�� �����ϸ� ���α׷� ����
			perror("fopen");
			exit(1);
		}
		char str[128];
		while (ptr != NULL)               // �ڸ� ���ڿ��� ������ ���� ������ �ݺ�
		{
			ptr = strtok(NULL, " ");      // ���� ���ڿ��� �߶� �����͸� ��ȯ
			sprintf(str, ptr);
			strcat(str, " ");
			fwrite(str, sizeof(char), strlen(str), ffd);
		}
		fwrite(" ", sizeof(char), strlen(" "), ffd);
		//fifo close
		close(fd);
		break;
	}
	close(ffd);
	//remove fifo file
	if (remove("./.myfifo") < 0) {
		perror("remove");
		exit(1);
	}

}
