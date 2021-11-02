#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");

	exit(0);
}

int
main()
{
	signal(SIGINT, SigIntHandler);//SIGINT signal handler ���

	printf("Press ^C to quit\n");

	for (; ; )
		pause();	//signal�� �߻��� ������ �ٸ� ���� ���� �ʰ� ��ٸ�
}
