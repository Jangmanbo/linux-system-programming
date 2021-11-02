#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


/*
[Program Name]  : wait program
[Description]   : parent process가 wait system call을 호출하여 child process가 종료될 때까지 기다림
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process 생성
		perror()        //에러 메세지 출력
	printf()	//메세지 출력
		exit()          //프로그램 종료
	wait()		//child process가 종료될 때까지 기다림
	sleep()		//지정한 시간동안 대기
*/
int
main()
{
	pid_t	pid;
	int		status;

	//child process 생성
	//parent process라면 pid에 child process의 PID 저장
	//child process라면 pid에 0 저장
	if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종료
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process의 경우
		printf("I'm a child\n");
		sleep(2);//2초간 대기
	}
	else {//parent process의 경우
		//&status : child process의 exit의 return value(child process가 종료될 때 status)
		//child process가 종료될 때까지 기다린 후 child process가 종료되었음을 출력
		wait(&status);
		printf("A child killed with %d status\n", status);
	}
}
