#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Function Name] : SigIntHandler
[Description]   : SIGINT signal이 발생하면 메시지 출력 후 프로그램 종료
[Input]         : signo	//수신한 signal
[Output]        : 출력 메세지
[Call By]	: kernel
[Calls]
	printf()	//메세지 출력
	exit()		//프로그램 종료
[Given]		: Nothing.
[Returns]	: Nothing.
*/
void
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");

	exit(0);
}
/*
[Program Name]  : signal program
[Description]   : SIGINT signal handler를 등록하고 signal이 발생할 때까지 대기
[Input]         : Nothing.
[Output]        : Nothing.
[Calls]
		signal()        //signal handler를 등록
	printf()	//메세지 출력
	pause()		//signal이 발생할 때까지 대기
*/
int
main()
{
	signal(SIGINT, SigIntHandler);//SIGINT signal handler 등록

	printf("Press ^C to quit\n");

	for (; ; )
		pause();	//signal이 발생할 때까지 다른 일을 하지 않고 기다림
}
