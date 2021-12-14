#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


//�ٸ� ���Ͽ��� ALarmSecs ���� ���� �Ұ�
static unsigned int	AlarmSecs;

/*
[Function Name] : SigAlarmHandler
[Description]   : SIGALRM�� �߻��Ǹ� ȣ��, 1���� SIGALRM�� ������ .�� ����
[Input]         : signo	//������ signal
[Output]        : . ���
[Call By]	: kernel
[Calls]
		signal()        //signal handler�� ���
		alarm()         //n�� �Ŀ� SIGALRM�� ����
	perror()	//���� �޼��� ���
	exit()		//���α׷� ����
	fflush()	//��� ���ۿ� �����ִ� �����͸� ��� ����ϰ� ���۸� ���
[Given]		:
	AlarmSecs	//�� �� �Ŀ� SIGALRM signal�� ���� ��
[Returns]	: Nothing.
*/
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
/*
[Function Name] : SetPeriodicAlarm
[Description]   : SIGALRM signal handler�� ����ϰ� SIGALRM signal�� ����
[Input]		: nsecs	//�� �� �Ŀ� signal�� ���� ��
[Output]        : Nothing.
[Call By]	: main()
[Calls]
	signal()	//signal handler�� ���
	alarm()		//n�� �Ŀ� SIGALRM�� ����
[Given]		: Nothing.
[Return]	:
	AlarmSecs	//�� �� �Ŀ� SIGALRM signal�� ���� ��
	return 0;
*/
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

/*
[Program Name]  : every one seconds do something
[Description]   : 1�ʸ��� Ư�� ���� �����Ű�� ���α׷�
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	printf()		//�޼��� ���
	SetPeriodicAlarm()	//SIGALRM signal handler�� ����ϰ� 1�� �Ŀ� SIGALRM�� ����
	pause()			//�ñ׳��� ������ ������ ���
*/
int
main()
{
	printf("Doing something every one seconds\n");

	//SIGALRM signal handler�� ����ϰ� 1�� �Ŀ� SIGALRM�� ����
	SetPeriodicAlarm(1);

	for (; ; )
		pause();//�ñ׳��� ������ ������ ���
}
