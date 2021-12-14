#include <stdio.h>
#include "synclib.h"
#include <stdlib.h>

#define	NLOOPS	5

/*
[Program Name]  : synchronization program
[Description]   : pipe�� �����Ͽ� parent, child process�� �޼����� �ְ� �޴� ���α׷�
[Input]		: Nothing.
[Output]	: Nothing.
[Calls]         :
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	printf()	//�޼��� ���
	fork()		//child process ����
	TELL_WAIT()	//pipe ����
	TELL_CHILD()	//child process���� �޼��� �ۼ�
	WAIT_CHILD()	//child process�� ��ٸ�, waiting ���·� ��ȯ, �޼����� ���� read
	TELL_PARENT()	//parent process���� �޼��� �ۼ�
	WAIT_PARENT()	//parent process�� ��ٸ�, waiting ���·� ��ȯ, �޼����� ���� read
*/
int
main()
{
	int		i;
	pid_t	pid;

	TELL_WAIT();	//pipe ����
	if ((pid = fork()) < 0) {//child process ����, �����ϸ� ���μ��� ����
		perror("fork");
		exit(1);
	}
	else if (pid > 0) {//parent process�� ���
		for (i = 0; i < NLOOPS; i++) {
			TELL_CHILD();	//child process���� �޼��� �ۼ�
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD();	//child process�� �޼����� �����⸦ ��ٸ� (running->waiting)
		}
	}
	else {	//child process�� ���
		for (i = 0; i < NLOOPS; i++) {
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT();//parent process�� �޼����� �����⸦ ��ٸ� (running->waiting)
			TELL_PARENT();//parent process���� �޼��� �ۼ�
			printf("Child: Tell to parent\n");
		}
	}
}
