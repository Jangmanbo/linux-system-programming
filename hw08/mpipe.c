#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define	MAX_BUF	128

/*
[Program Name]  : pipe program
[Description]   : pipe�� �����Ͽ� parent, child process�� ���� �����͸� �ְ� �޴� ���α׷�
[Input]		: Nothing.
[Output]	: �ٸ� ���μ����� write�� data�� read�Ͽ� ���
[Calls]         :
	pipe()		//pipe ����
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	printf()	//�޼��� ���
	fork()		//child process ����
	close()		//file descriptor close
	read()		//read file, ���� ����Ʈ ���� return
	fflush()	//���ۿ� ����� �����͸� ����ϰ� ����
	write()		//file�� write
	strcpy()	//���ڿ� ����
*/
int
main()
{
	int	n, fd1[2], fd2[2];
	pid_t	pid;
	char	buf1[MAX_BUF], buf2[MAX_BUF];

	//pipe ����
	if (pipe(fd1) < 0 || pipe(fd2) < 0) {
		perror("pipe");
		exit(1);
	}

	//process fork
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	//child process�� ���
	else if (pid == 0) {
		//read, write ��� �ؾ� �ϹǷ� �� �������� write, read fd close
		close(fd1[1]);
		close(fd2[0]);

		//buf2�� ���ڿ� ����
		strcpy(buf2, "Child process sends data\n");
		printf("Child: Send data to parent\n");
		//fd2[1]�� ������ write
		write(fd2[1], buf2, strlen(buf2) + 1);

		printf("Child : Wait for parent to send data\n");
		if ((n = read(fd1[0], buf1, MAX_BUF)) < 0) {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout);
		//ȭ�鿡read data ���
		write(STDOUT_FILENO, buf1, n);
	}
	else {
		//read, write ��� �ؾ� �ϹǷ� �� �������� write, read fd close
		close(fd1[0]);
		close(fd2[1]);
		//buf�� ���ڿ� ����
		strcpy(buf1, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		//fd1[1]�� ������ write
		write(fd1[1], buf1, strlen(buf1) + 1);

		printf("Parent : Wait for parent to send data\n");
		if ((n = read(fd2[0], buf2, MAX_BUF)) < 0) {
			perror("read");
			exit(1);
		}
		printf("Parent : Received data from child: ");
		fflush(stdout);
		//ȭ�鿡 read data ���
		write(STDOUT_FILENO, buf2, n);
	}

	exit(0);	//process ����
}
