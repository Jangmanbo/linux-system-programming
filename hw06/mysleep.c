#include <stdio.h>
#include <signal.h>
#include <unistd.h>

//5�� �� SIGALRM�� ���� ���μ��� ����

void
SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	return;
}

unsigned int
mysleep(unsigned int nsecs)
{
	//SIGALRM signal handler ���
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		return nsecs;
	}

	alarm(5);	//5�� �� SIGALRM signal ����

	pause();

	return alarm(0);
}

int
main()
{
	printf("Wait for 5 seconds...\n");

	mysleep(5);
}
