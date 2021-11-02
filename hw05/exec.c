#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : program execute
[Description]   : child process를 생성하여 여러 방법으로 다른 프로그램을 실행
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process 생성
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
	execl()		//argv를 list로 받아 프로그램 실행
	execv()		//argv를 vector로 받아 프로그램 실행
	execlp()	//path에 저장된 경로에서 실행파일을 찾아서 실행
	waitpid()	//PID가 pid인 child process가 종료될 떄까지 기다림
*/
int
main()
{
	pid_t	pid;
	char* argv[] = { "mycp", "exec.c", "exec2.c", NULL };

	//child process 생성
	//parent process라면 pid에 child process의 PID 저장
	//child process라면 pid에 0 저장
	if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종료
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process의 경우
		//execl 방법으로 mycp 프로그램 실행
		//NULL로 argv list의 끝임을 표시
		if (execl("/home/lsp36/practice/hw05/mycp",
			"mycp", "exec.c", "exec1.c", NULL) < 0) {//실패하면 -1 return, 프로그램 종료
			perror("execl");
			exit(1);
		}
	}
	//PID가 pid인 child process가 종료될 떄까지 기다림
	if (waitpid(pid, NULL, 0) < 0) {
		perror("waitpid");
		exit(1);
	}

	//child process 생성
		//parent process라면 pid에 child process의 PID 저장
		//child process라면 pid에 0 저장
	if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종료
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process의 경우
			//execv 방법으로 mycp 프로그램 실행
				//미리 선언된 argv 배열 사용
		if (execv("/home/lsp36/practice/hw05/mycp", argv) < 0) {//실패하면 -1 return, 프로그램 종료
			perror("execv");
			exit(1);
		}
	}

	//PID가 pid인 child process가 종료될 떄까지 기다림
	if (waitpid(pid, NULL, 0) < 0) {
		perror("waitpid");
		exit(1);
	}

	//child process 생성
		//parent process라면 pid에 child process의 PID 저장
		//child process라면 pid에 0 저장
	if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종료
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process의 경우
				//execlp
		//path에서 env를 찾아 실행
		if (execlp("env", "env", NULL) < 0) {
			perror("execlp");
			exit(1);
		}
	}

	//PID가 pid인 child process가 종료될 떄까지 기다림
	if (waitpid(pid, NULL, 0) < 0) {
		perror("waitpid");
		exit(1);
	}
}
