#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
mysystem(char* cmd) {
	int	status;
	pid_t	pid;
	char* argv[] = { "sh", "-c", cmd, NULL };

	//child process 생성
		//parent process라면 pid에 child process의 PID 저장
		//child process라면 pid에 0 저장
	if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종료
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {//child process의 경우
		if (execv("/bin/sh", argv) < 0) {//실행 파일의 디렉토리 전체 경로와 argv 배열을 통해 다른 프로그램을 실행, 실패하면 프로그램 종료
			perror("execv");
			exit(1);
		}
	}

	//process id가 pid인 process의 종료를 기다림, 실패하면 프로그램 종료
	if (waitpid(pid, &status, 0) < 0) {
		perror("waitpid");
		exit(1);
	}

	//command 실행에 성공하면 command의 return value를 return
	//실패하면 -1 return
	return status;
}

/*
[Program Name]  : system program
[Description]   : mysystem 함수를 호출하여 command 실행(child process로 command를 실행하고 child가 끝날 때까지 기다렸다가 끝나면 다음 command 실행)
[Input]		: Nothing.
[Output]        : command 실행 결과
[Calls]
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
	system()	//파라미터로 받은 명령어를 실행, 명령어의 return값을 return, 실패하면 -1 return
	printf		//메세지 출력
*/
int
main()
{
	int		status;

	//status에 date command의 return값이 저장
	if ((status = mysystem("date")) < 0) {//실패하면 프로그램 종료
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//nosuchcommand라는 command는 존재하지 않으므로 status에 -1이 저장, 프로그램 종료
	if ((status = mysystem("nosuchcommand")) < 0) {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//status에 who와 exit 44 두 command의 return값이 저장
	if ((status = mysystem("who; exit 44")) < 0) {//실패하면 프로그램 종료
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//mycp 프로그램 실행 (추가 예제)
	if ((status = mysystem("./mycp exec.c exec2.c")) < 0) {//실패하면 프로그램 종료
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

}
