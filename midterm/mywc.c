#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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
		char* args[4] = { "wc", "-w", argv[1], NULL };
		if (execvp("wc", args) < 0) {
			perror("execvp");
			exit(1);
		}
	}
	else {//parent process의 경우
		//&status : child process의 exit의 return value(child process가 종료될 때 status)
		//child process가 종료될 때까지 기다린 후 child process가 종료되었음을 출력
		wait(NULL);
	}





}
