#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Function Name] : SigUsrHandler
[Description]   : signal�� ���� �ٸ��� ����
[Input]         : signo //������ signal
[Output]        : ��� signal�� �����Ͽ����� ���
[Call By]	: kernel
[Calls]
		printf()        //�޼��� ���
		exit()          //���α׷� ����
[Given]		: Nothing.
[Returns]	: Nothing.
*/
void
SigUsrHandler(int signo)
{
	if (signo == SIGUSR1) {//SIGUSR1 signal�� ���
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2) {//SIGUSR2 signal�� ���
		printf("Received a SIGUSR2 signal\n");
	}
	else {//SIGUSR1, SIGUSR2 ���� signal�� ��� ���α׷� ����
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

/*
[Program Name]  : signal program
[Description]   : signal handler�� ����ϰ� signal�� �߻��� ������ ���
[Input]         : Nothing.
[Output]        : Nothing.
[Calls]
		signal()        //signal handler�� ���
		perror()	//���� �޼��� ���
	pause()         //signal�� �߻��� ������ ���
	exit()		//���α׷� ����
*/

int
main()
{
	//SIGUSR1 signal handler ���
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	//SIGUSR2 signal handler ���
	if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	for (; ; )
		pause();	//signal�� �߻��� ������ �ٸ� ���� ���� �ʰ� ��ٸ�
}
