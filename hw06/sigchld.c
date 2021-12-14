#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*
[Function Name] : SigChildHandler
[Description]   : SIGCHLD이 발생되면 호출되어 프로세스 종료
[Input]         : signo //수신한 signal
[Output]        : signal handler 호출 메세지 출력
[Call By]       : kernel
[Calls]
		printf()	//메세지 출력
		exit()          //프로그램 종료
[Given]         :
[Returns]       : Nothing.
*/
void SigChildHandler(int signo)
{
	printf("SIGCHLD signal handler!\n");
	exit(0);
}
/*
[Program Name]  : wait program
[Description]   : Parent process가 child process의 종료시, SIGCHLD signal을 asynchronous하게 처리하는 프로그램
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	signal()	//signahl handler 등록
	fork()		//child process 생성
		perror()        //에러 메세지 출력
	printf()	//메세지 출력
		exit()          //프로그램 종료
	sleep()		//지정한 시간동안 대기
*/
int
main()
{
	pid_t	pid;

	//SIGCHLD sighandler 등록
	if (signal(SIGCHLD, SigChildHandler) == SIG_ERR) {
		return -1;
	}

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
		while (1)
			;
	}
}
