#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//전역변수
int		Var = 6;
char	Buf[] = "a write to stdout\n";

/*
[Program Name]  : fork program
[Description]   : fork로 child process를 생성
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process 생성
		perror()        //에러 메세지 출력
	printf()	//메세지 출력
		exit()          //프로그램 종료
	write()		//파일에 데이터 쓰기
	sleep()		//지정한 시간동안 대기
	getpid()	//함수를 호출한 프로세스의 PID를return
*/
int
main()
{
	//로컬변수
	int		var;
	pid_t	pid;

	var = 88;
	write(STDOUT_FILENO, Buf, sizeof(Buf) - 1);//STDOUT_FILENO에 Buf 출력, 즉 printf와 동일
	printf("Before fork\n");

	//child process 생성
	//parent process라면 pid에 child process의 PID 저장
	//child process라면 pid에 0 저장
	if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종료
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {//child process의 경우
		/* child */
		//child process의 Var, var 변수의 값 변경
		Var++;
		var++;
	}
	else {//parent process의 경우
		/* parent */
		sleep(2);//parent process는 2초동안 대기
	}

	//자신의 PID와 Var, var변수의 값 출력
	//child process의 경우 Var=7, var=89가 출력
	//parent process의 경우 Var, var의 값의 변동이 없으므로Var=6, var=88이 출력
	//parent process가 2초간 대기하므로 child process가 먼저 출력
	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);
}
