#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/*
[Program Name]  : wc -w program
[Description]   : word 개수를 출력하는 wc -w command와 같은 기능을 수행하는 프로그램
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 파일과 문자
[Output]        : argv[1] 파일에argv[2]가 몇 개 있는지
[Calls]         :
	fprintf()	//파일에 메세지 출력
	exit()		//프로세스 종료
	perror()	//에러 메세지 출력
	execvp()	//path에 저장된 경로에서 실행파일을 찾아서 실행
	wait()		//child process가 종료될 때까지 대기
*/
int
main(int argc, char* argv[])
{
	int pid;

	if (argc != 2) {//argv!=3이면 프로그램 종료
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종료
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process의 경우
		//execvp 인수 배열 선언
		char* args[4] = { "wc", "-w", argv[1], NULL };
		//path에 저장된 경로에서 실행파일을 찾아서 실행
		if (execvp("wc", args) < 0) {
			//실패하면 프로세스 종료
			perror("execvp");
			exit(1);
		}
	}
	else {//parent process의 경우
		//child process가 종료될 때까지 대기
		wait(NULL);
	}

}
