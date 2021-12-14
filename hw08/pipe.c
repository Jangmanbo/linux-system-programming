#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define	MAX_BUF	128

/*
[Program Name]  : pipe program
[Description]   : pipe를 생성하여 parent, child process가 데이터를 주고 받는 프로그램
[Input]		: Nothing.
[Output]	: parent가 보낸 read data
[Calls]         :
	pipe()		//pipe 생성
	exit()		//프로세스 종료
	perror()	//에러 메세지 출력
	printf()	//메세지 출력
	fork()		//child process 생성
	close()		//file descriptor close
	read()		//read file, 읽은 바이트 수를 return
	fflush()	//버퍼에 저장된 데이터를 출력하고 비우기
	write()		//file에 write
	strcpy()	//문자열 복사
*/
int
main()
{
	int		n, fd[2];
	pid_t	pid;
	char	buf[MAX_BUF];

	//pipe 생성
	if (pipe(fd) < 0) {
		perror("pipe");
		exit(1);
	}

	//process fork
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	//child process의 경우
	else if (pid == 0) {
		//child는 read하므로 write을 하는 fd[1] close
		close(fd[1]);
		printf("Child : Wait for parent to send data\n");
		//fd[0]으로 buf에서 데이터 read
		if ((n = read(fd[0], buf, MAX_BUF)) < 0) {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		//출력 버퍼에 있는 내용을 출력하고 출력 버퍼 비움
		fflush(stdout);
		//화면에read data 출력
		write(STDOUT_FILENO, buf, n);
	}
	//parent process의 경우
	else {
		//parent는 write하므로 read를 하는 fd[0] close
		close(fd[0]);
		//buf에 문자열 복사
		strcpy(buf, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		//fd[1]으로 데이터 write
		write(fd[1], buf, strlen(buf) + 1);
	}

	exit(0);	//process 종료
}
