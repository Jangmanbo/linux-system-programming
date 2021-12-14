#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Function Name] : SigUsrHandler
[Description]   : signal에 따라 다르게 동작
[Input]         : signo //수신한 signal
[Output]        : 어던 signal을 수신하였는지 출력
[Call By]	: kernel
[Calls]
		printf()        //메세지 출력
		exit()          //프로그램 종료
[Given]		: Nothing.
[Returns]	: Nothing.
*/
void
SigUsrHandler(int signo)
{
	if (signo == SIGUSR1) {//SIGUSR1 signal인 경우
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2) {//SIGUSR2 signal인 경우
		printf("Received a SIGUSR2 signal\n");
	}
	else {//SIGUSR1, SIGUSR2 외의 signal인 경우 프로그램 종료
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

/*
[Program Name]  : signal program
[Description]   : signal handler를 등록하고 signal이 발생할 때까지 대기
[Input]         : Nothing.
[Output]        : Nothing.
[Calls]
		signal()        //signal handler를 등록
		perror()	//에러 메세지 출력
	pause()         //signal이 발생할 때까지 대기
	exit()		//프로그램 종료
*/

int
main()
{
	//SIGUSR1 signal handler 등록
	if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	//SIGUSR2 signal handler 등록
	if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	for (; ; )
		pause();	//signal이 발생할 때까지 다른 일을 하지 않고 기다림
}
