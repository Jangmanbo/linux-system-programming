#include <stdio.h>
#include <signal.h>
#include <unistd.h>


/*
[Function Name]  : SigAlarmHandler
[Description]   : SIGALRM이 발생되면 호출
[Input]         : signo	//수신한 signal
[Output]        : Nothing.
[Call By]	: kernel
[Calls]		: Nothing.
[Given]		: Nothing.
[Return]	: Nothing.
*/
void
SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	return;
}

/*
[Function Name] : mysleep
[Description]   : SIGALRM signal handler를 등록하고 5초 후에 SIGALRM을 보냄
[Input]         :
	nsecs	//몇초 후에 SIGALRM을 보낼지
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
	signal()        //signal handler를 등록
		alarm()         //n초 후에 SIGALRM을 보냄
	pause()		//signal이 발생할 때까지 대기
[Given]         : Nothing.
[Return]        : 최근에 설정한 alarm이 몇초 남았는지
*/
unsigned int
mysleep(unsigned int nsecs)
{
	//SIGALRM signal handler 등록
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		return nsecs;
	}

	alarm(5);	//5초 후 SIGALRM signal 보냄

	pause();	//signal이 발생할 때까지 대기

	//최근에 설정한 alarm이 몇 초 남았는지return
	//SIGALRM이 발생하기 전까지 다른 signal이 없었다면 0을 return
	//SIGALRM이 발생하기 전에 n초 후 다른 signal이 발생하면 5-n을 return
	return alarm(0);
}
/*
[Program Name]  : signal program
[Description]   : 5초 후 SIGALRM을 보내 프로세스 종료
[Input]         : Nothing.
[Output]        : 안내 메세지
[Calls]
	printf()	//메세지 출력
	mysleep()	//SIGALRM signal handler를 등록하고 5초 후에 SIGALRM을 보냄
*/
int
main()
{
	printf("Wait for 5 seconds...\n");


	//SIGALRM signal handler를 등록하고 5초 후에 SIGALRM을 보냄
	mysleep(5);
}
