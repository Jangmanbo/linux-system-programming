#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define	MAX_BUF	128

/*
[Program Name]  : pipe program
[Description]   : pipe를 생성하여 parent, child process가 서로 데이터를 주고 받는 프로그램
[Input]		: Nothing.
[Output]	: 다른 프로세스가 write한 data를 read하여 출력
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
	int	n, fd1[2], fd2[2];
	pid_t	pid;
	char	buf1[MAX_BUF], buf2[MAX_BUF];

	//pipe 생성
	if (pipe(fd1) < 0 || pipe(fd2) < 0) {
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
		//read, write 모두 해야 하므로 각 파이프의 write, read fd close
		close(fd1[1]);
		close(fd2[0]);

		//buf2에 문자열 복사
		strcpy(buf2, "Child process sends data\n");
		printf("Child: Send data to parent\n");
		//fd2[1]에 데이터 write
		write(fd2[1], buf2, strlen(buf2) + 1);

		printf("Child : Wait for parent to send data\n");
		if ((n = read(fd1[0], buf1, MAX_BUF)) < 0) {
			perror("read");
			exit(1);
		}
		printf("Child : Received data from parent: ");
		fflush(stdout);
		//화면에read data 출력
		write(STDOUT_FILENO, buf1, n);
	}
	else {
		//read, write 모두 해야 하므로 각 파이프의 write, read fd close
		close(fd1[0]);
		close(fd2[1]);
		//buf에 문자열 복사
		strcpy(buf1, "Hello, World!\n");
		printf("Parent: Send data to child\n");
		//fd1[1]에 데이터 write
		write(fd1[1], buf1, strlen(buf1) + 1);

		printf("Parent : Wait for parent to send data\n");
		if ((n = read(fd2[0], buf2, MAX_BUF)) < 0) {
			perror("read");
			exit(1);
		}
		printf("Parent : Received data from child: ");
		fflush(stdout);
		//화면에 read data 출력
		write(STDOUT_FILENO, buf2, n);
	}

	exit(0);	//process 종료
}
