#include <stdio.h>
#include <signal.h>
#include <unistd.h>

//5초 후 SIGALRM을 보내 프로세스 종료

void
SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	return;
}

unsigned int
mysleep(unsigned int nsecs)
{
	//SIGALRM signal handler 등록
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		return nsecs;
	}

	alarm(5);	//5초 후 SIGALRM signal 보냄

	pause();

	return alarm(0);
}

int
main()
{
	printf("Wait for 5 seconds...\n");

	mysleep(5);
}
