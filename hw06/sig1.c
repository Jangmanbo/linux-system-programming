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
	signal(SIGINT, SigIntHandler);//SIGINT signal handler 등록

	printf("Press ^C to quit\n");

	for (; ; )
		pause();	//signal이 발생할 때까지 다른 일을 하지 않고 기다림
}
