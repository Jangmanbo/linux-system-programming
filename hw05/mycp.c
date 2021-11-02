#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define	MAX_BUF	1024

/*
[Program Name]	: copy program
[Description]	: argv[1] ������ argv[2]���Ϸ� copy
[Input]
	int argc	// argv �迭�� ũ��
	char *argv[] 	// ������ ���α׷�, ������ ����, �����Ͽ� ���� ���� ������ ������
[Output]	: �����Ͽ� ���� ���� ����
[Calls]		: printf, open, read, write, close
*/

int
main(int argc, char* argv[])
{
	int 	fd1, fd2, count;
	char	buf[MAX_BUF];

	if (argc != 3) {//argv!=3�̸� ���α׷� ����
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//argv[1]������ ������ ���̹Ƿ� read only
	if ((fd1 = open(argv[1], O_RDONLY)) < 0) {//argv[1]������ �б⿡ ����
		perror("open");
		exit(1);
	}

	/*	argv[2]���Ͽ� ������ ���̹Ƿ�
		O_WRONLY:���Ͽ� ����
		O_CREAT:���� ����
		O_TRUNC:argv[2]������ �̹� �־��ٸ� ������ �ִ� ���� ����
		0644 : �����ڴ� �б�� ����, �׷�� ��Ÿ ����ڴ� �б� ����*/
	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {//argv[2]���Ͽ� �����ϱ� ����
		perror("open");
		exit(1);
	}

	while (count = read(fd1, buf, MAX_BUF)) {//fd1�� �� ������count=0���� ���� ��������
		write(fd2, buf, count);//buf�� ������ fd2�� count byte��ŭwrite
	}

	close(fd1);
	close(fd2);
}
