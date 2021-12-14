#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Program Name]  : duplicate program
[Description]   : file descriptor�� STDOUT, STDIN�� duplicate�Ͽ� command �������� ���
[Input]		: Nothing.
[Output]	: ls -l | more command ������
[Calls]         :
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	pipe()		//pipe ����
	printf()	//�޼��� ���
	fork()		//child process ����
	close()		//file descriptor close
	dup()		//file descriptor ���纻 ����, ������ �ʴ� file descriptor �� ���� ���� ���ڸ� ���ο� file descriptor������ ���
	dup2()		//file descriptor ���纻 ����, ���ο� file descriptor�� ����
	execlp()	//path ȯ�溯������ ã�� command ����, �Ķ���͸� list�� ����
*/
int
main()
{
	int fd[2];
	pid_t child;

	//pipe ����
	if (pipe(fd) == -1) {
		perror("pipe");
		exit(1);
	}

	//fork system call�� child process ����
	child = fork();
	if (child == (pid_t)(-1)) {//�����ϸ� ���μ��� ����
		perror("fork");
		exit(1);
	}
	else if (child == (pid_t)0) {//child process
		close(1);	//1�� file descriptor(=STDOUT) close
		close(fd[0]);	//����ϹǷ� read�� �ϴ� fd[0]�� close

		//�ռ� 1�� file descriptor�� close
		//0������ �Ⱦ� close�� 1�� file descriptor��fd[1]�� duplicate
		if (dup(fd[1]) == -1) {
			perror("dup");
			exit(1);
		}

		//path ȯ�溯������ ls�� ã�� command ����
		if ((execlp("ls", "ls", "-l", NULL)) == -1) {
			perror("execlp");
			exit(1);
		}
	}
	else {//parent process
		close(0);	//0�� file descriptor(=STDIN) close
		close(fd[1]);	//�Է��ϹǷ� write�� �ϴ� fd[1]�� close

		//0�� file descriptor�� fd[0]�� duplicate
		if (dup2(fd[0], 0) == -1) {
			perror("dup");
			exit(1);
		}

		//path ȯ�溯������ more�� ã�� command ����
		if ((execlp("more", "more", NULL)) == -1) {
			perror("execlp");
			exit(1);
		}
	}

	return 0;
}