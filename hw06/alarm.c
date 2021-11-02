#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


//SIGALRM signal�� �̿��ؼ� 1�ʿ� �ϳ��� ���� ���



//�ٸ� ���Ͽ��� ALarmSecs ���� ���� �Ұ�
static unsigned int	AlarmSecs;


void
SigAlarmHandler(int signo)
{
	//SIGALRM sighandler ���(linux������ ���ʿ�)
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	alarm(AlarmSecs); //1�� �� SIGALRM signal ����

	/* Do something */
	printf(".");

	//stdout ���� ĳ�ø� ���
	//ȭ�鿡 ���� �ϳ��� ����
	fflush(stdout);

	return;
}

int
SetPeriodicAlarm(unsigned int nsecs)
{
	//SIGALRM sighandler ���
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		return -1;
	}

	AlarmSecs = nsecs;

	//1�� �� SIGALRM signal ����
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
