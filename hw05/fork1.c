#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : fork program
[Description]   : fork로 child process를 생성하고 parent, child process의 PID 출력
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process 생성
		perror()        //에러 메세지 출력
	printf()	//메세지 출력
		exit()          //프로그램 종료
	getpid()	//함수를 호출한 프로세스의 PID를return
	getppid()	//parent process의 PID를 return
*/
int
main()
{
	int		pid;

	//child process 생성
	//parent process라면 pid에 child process의 PID 저장
	//child process라면 pid에 0 저장
	if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종//child process의 경우
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process의 경우
		/* child */
		//자신의 PID와 parent process의 PID 출력
		printf("I am %d. My parent is %d.\n", getpid(), getppid());
	}
	else {//parent process의 경우
		/* parent */
		//자신의 PID와 child process의 PID 출력
		printf("I am %d. My child is %d.\n", getpid(), pid);
	}
}
