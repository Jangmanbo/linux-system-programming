#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


//SIGALRM signal을 이용해서 1초에 하나의 점을 출력



//다른 파일에서 ALarmSecs 변수 접근 불가
static unsigned int	AlarmSecs;


void
SigAlarmHandler(int signo)
{
	//SIGALRM sighandler 등록(linux에서는 불필요)
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	alarm(AlarmSecs); //1초 후 SIGALRM signal 보냄

	/* Do something */
	printf(".");

	//stdout 버퍼 캐시를 비움
	//화면에 점이 하나씩 찍힘
	fflush(stdout);

	return;
}

int
SetPeriodicAlarm(unsigned int nsecs)
{
	//SIGALRM sighandler 등록
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		return -1;
	}

	AlarmSecs = nsecs;

	//1초 후 SIGALRM signal 보냄
	alarm(nsecs);

	return 0;
}

int
main()
{
	printf("Doing something every one seconds\n");

	SetPeriodicAlarm(1);

	for (; ; )
		pause();
}
