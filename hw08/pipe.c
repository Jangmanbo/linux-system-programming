#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define	MAX_BUF	128

/*
[Program Name]  : pipe program
[Description]   : pipe�� �����Ͽ� parent, child process�� �����͸� �ְ� �޴� ���α׷�
[Input]		: Nothing.
[Output]	: parent�� ���� read data
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
	int		n, fd[2];
	pid_t	pid;
	char	buf[MAX_BUF];

	//pipe ����
	if (pipe(fd) < 0) {
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
		//child�� read�ϹǷ� write�� �ϴ� fd[1] close
		close(fd[1]);
		printf("Child : Wait for parent to send data\n");
		//fd[0]���� buf���� ������ read
		if ((n = read(fd[0], buf, MAX_BUF)) < 0) {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		//��� ���ۿ� �ִ� ������ ����ϰ� ��� ���� ���
		fflush(stdout);
		//ȭ�鿡read data ���
		write(STDOUT_FILENO, buf, n);
	}
	//parent process�� ���
	else {
		//parent�� write�ϹǷ� read�� �ϴ� fd[0] close
		close(fd[0]);
		//buf�� ���ڿ� ����
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		//fd[1]���� ������ write
		write(fd[1], buf, strlen(buf) + 1);
	}

	exit(0);	//process ����
}
