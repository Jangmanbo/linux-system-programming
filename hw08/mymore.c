#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Program Name]  : duplicate program
[Description]   : file descriptor를 STDOUT, STDIN과 duplicate하여 command 실행결과를 출력
[Input]		: Nothing.
[Output]	: ls -l | more command 실행결과
[Calls]         :
	exit()		//프로세스 종료
	perror()	//에러 메세지 출력
	pipe()		//pipe 생성
	printf()	//메세지 출력
	fork()		//child process 생성
	close()		//file descriptor close
	dup()		//file descriptor 복사본 생성, 사용되지 않는 file descriptor 중 가장 낮은 숫자를 새로운 file descriptor값으로 사용
	dup2()		//file descriptor 복사본 생성, 새로운 file descriptor를 지정
	execlp()	//path 환경변수에서 찾아 command 실행, 파라미터를 list로 받음
*/
int
main()
{
	int fd[2];
	pid_t child;

	//pipe 생성
	if (pipe(fd) == -1) {
		perror("pipe");
		exit(1);
	}

	//fork system call로 child process 생성
	child = fork();
	if (child == (pid_t)(-1)) {//실패하면 프로세스 종료
		perror("fork");
		exit(1);
	}
	else if (child == (pid_t)0) {//child process
		close(1);	//1번 file descriptor(=STDOUT) close
		close(fd[0]);	//출력하므로 read를 하는 fd[0]은 close

		//앞서 1번 file descriptor를 close
		//0번부터 훑어 close된 1번 file descriptor와fd[1]이 duplicate
		if (dup(fd[1]) == -1) {
			perror("dup");
			exit(1);
		}

		//path 환경변수에서 ls를 찾아 command 실행
		if ((execlp("ls", "ls", "-l", NULL)) == -1) {
			perror("execlp");
			exit(1);
		}
	}
	else {//parent process
		close(0);	//0번 file descriptor(=STDIN) close
		close(fd[1]);	//입력하므로 write을 하는 fd[1]은 close

		//0번 file descriptor를 fd[0]에 duplicate
		if (dup2(fd[0], 0) == -1) {
			perror("dup");
			exit(1);
		}

		//path 환경변수에서 more를 찾아 command 실행
		if ((execlp("more", "more", NULL)) == -1) {
			perror("execlp");
			exit(1);
		}
	}

	return 0;
}