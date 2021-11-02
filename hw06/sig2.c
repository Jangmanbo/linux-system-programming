#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//signal에 따라 다르게 동작

void
SigUsrHandler(int signo)
{
	if (signo == SIGUSR1) {
		printf("Received a SIGUSR1 signal\n");
	}
	else if (signo == SIGUSR2) {
		printf("Received a SIGUSR2 signal\n");
	}
	else {
		printf("Received unknown signal\n");
		printf("Terminate this process\n");
		exit(0);
	}
}

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
