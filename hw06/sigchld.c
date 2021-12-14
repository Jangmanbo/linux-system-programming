#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*
[Function Name] : SigChildHandler
[Description]   : SIGCHLD�� �߻��Ǹ� ȣ��Ǿ� ���μ��� ����
[Input]         : signo //������ signal
[Output]        : signal handler ȣ�� �޼��� ���
[Call By]       : kernel
[Calls]
		printf()	//�޼��� ���
		exit()          //���α׷� ����
[Given]         :
[Returns]       : Nothing.
*/
void SigChildHandler(int signo)
{
	printf("SIGCHLD signal handler!\n");
	exit(0);
}
/*
[Program Name]  : wait program
[Description]   : Parent process�� child process�� �����, SIGCHLD signal�� asynchronous�ϰ� ó���ϴ� ���α׷�
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	signal()	//signahl handler ���
	fork()		//child process ����
		perror()        //���� �޼��� ���
	printf()	//�޼��� ���
		exit()          //���α׷� ����
	sleep()		//������ �ð����� ���
*/
int
main()
{
	pid_t	pid;

	//SIGCHLD sighandler ���
	if (signal(SIGCHLD, SigChildHandler) == SIG_ERR) {
		return -1;
	}

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
		while (1)
			;
	}
}
