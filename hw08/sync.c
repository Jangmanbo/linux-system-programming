#include <stdio.h>
#include "synclib.h"
#include <stdlib.h>

#define	NLOOPS	5

/*
[Program Name]  : synchronization program
[Description]   : pipe를 생성하여 parent, child process가 메세지를 주고 받는 프로그램
[Input]		: Nothing.
[Output]	: Nothing.
[Calls]         :
	exit()		//프로세스 종료
	perror()	//에러 메세지 출력
	printf()	//메세지 출력
	fork()		//child process 생성
	TELL_WAIT()	//pipe 생성
	TELL_CHILD()	//child process에게 메세지 작성
	WAIT_CHILD()	//child process를 기다림, waiting 상태로 전환, 메세지가 오면 read
	TELL_PARENT()	//parent process에게 메세지 작성
	WAIT_PARENT()	//parent process를 기다림, waiting 상태로 전환, 메세지가 오면 read
*/
int
main()
{
	int		i;
	pid_t	pid;

	TELL_WAIT();	//pipe 생성
	if ((pid = fork()) < 0) {//child process 생성, 실패하면 프로세스 종료
		perror("fork");
		exit(1);
	}
	else if (pid > 0) {//parent process의 경우
		for (i = 0; i < NLOOPS; i++) {
			TELL_CHILD();	//child process에게 메세지 작성
			printf("Parent: Tell to child\n");
			printf("Parent: Wait for child to tell\n");
			WAIT_CHILD();	//child process가 메세지를 보내기를 기다림 (running->waiting)
		}
	}
	else {	//child process의 경우
		for (i = 0; i < NLOOPS; i++) {
			printf("Child: Wait for parent to tell\n");
			WAIT_PARENT();//parent process가 메세지를 보내기를 기다림 (running->waiting)
			TELL_PARENT();//parent process에게 메세지 작성
			printf("Child: Tell to parent\n");
		}
	}
}
