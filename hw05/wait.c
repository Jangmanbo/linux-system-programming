#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


/*
[Program Name]  : wait program
[Description]   : parent process�� wait system call�� ȣ���Ͽ� child process�� ����� ������ ��ٸ�
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process ����
		perror()        //���� �޼��� ���
	printf()	//�޼��� ���
		exit()          //���α׷� ����
	wait()		//child process�� ����� ������ ��ٸ�
	sleep()		//������ �ð����� ���
*/
int
main()
{
	pid_t	pid;
	int		status;

	//child process ����
	//parent process��� pid�� child process�� PID ����
	//child process��� pid�� 0 ����
	if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ����
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process�� ���
		printf("I'm a child\n");
		sleep(2);//2�ʰ� ���
	}
	else {//parent process�� ���
		//&status : child process�� exit�� return value(child process�� ����� �� status)
		//child process�� ����� ������ ��ٸ� �� child process�� ����Ǿ����� ���
		wait(&status);
		printf("A child killed with %d status\n", status);
	}
}
