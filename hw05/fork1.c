#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : fork program
[Description]   : fork�� child process�� �����ϰ� parent, child process�� PID ���
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process ����
		perror()        //���� �޼��� ���
	printf()	//�޼��� ���
		exit()          //���α׷� ����
	getpid()	//�Լ��� ȣ���� ���μ����� PID��return
	getppid()	//parent process�� PID�� return
*/
int
main()
{
	int		pid;

	//child process ����
	//parent process��� pid�� child process�� PID ����
	//child process��� pid�� 0 ����
	if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ��//child process�� ���
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process�� ���
		/* child */
		//�ڽ��� PID�� parent process�� PID ���
		printf("I am %d. My parent is %d.\n", getpid(), getppid());
	}
	else {//parent process�� ���
		/* parent */
		//�ڽ��� PID�� child process�� PID ���
		printf("I am %d. My child is %d.\n", getpid(), pid);
	}
}
