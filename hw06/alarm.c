#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


//다른 파일에서 ALarmSecs 변수 접근 불가
static unsigned int	AlarmSecs;

/*
[Function Name] : SigAlarmHandler
[Description]   : SIGALRM이 발생되면 호출, 1초후 SIGALRM을 보내고 .을 찍음
[Input]         : signo	//수신한 signal
[Output]        : . 출력
[Call By]	: kernel
[Calls]
		signal()        //signal handler를 등록
		alarm()         //n초 후에 SIGALRM을 보냄
	perror()	//에러 메세지 출력
	exit()		//프로그램 종료
	fflush()	//출력 버퍼에 남아있는 데이터를 모두 출력하고 버퍼를 비움
[Given]		:
	AlarmSecs	//몇 초 후에 SIGALRM signal을 보낼 지
[Returns]	: Nothing.
*/
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
/*
[Function Name] : SetPeriodicAlarm
[Description]   : SIGALRM signal handler를 등록하고 SIGALRM signal을 보냄
[Input]		: nsecs	//몇 초 후에 signal을 보낼 지
[Output]        : Nothing.
[Call By]	: main()
[Calls]
	signal()	//signal handler를 등록
	alarm()		//n초 후에 SIGALRM을 보냄
[Given]		: Nothing.
[Return]	:
	AlarmSecs	//몇 초 후에 SIGALRM signal을 보낼 지
	return 0;
*/
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

/*
[Program Name]  : every one seconds do something
[Description]   : 1초마다 특정 일을 수행시키는 프로그램
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	printf()		//메세지 출력
	SetPeriodicAlarm()	//SIGALRM signal handler를 등록하고 1초 후에 SIGALRM을 보냄
	pause()			//시그널을 수신할 때까지 대기
*/
int
main()
{
	printf("Doing something every one seconds\n");

	//SIGALRM signal handler를 등록하고 1초 후에 SIGALRM을 보냄
	SetPeriodicAlarm(1);

	for (; ; )
		pause();//시그널을 수신할 때까지 대기
}
