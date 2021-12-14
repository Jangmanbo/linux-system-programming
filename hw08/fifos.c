#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "fifo.h"


/*
[Function Name] : SigIntHandler
[Description]
	SIGINT signal이 발생하면 호출되는 함수
	fifo file을 삭제하고 프로세스를 종료시킴
[Input]         : signo	//수신한 signal
[Output]        : Nothing.
[Call By]	: kernel
[Calls]
	perror()	//에러 메세지 출력
	exit()		//프로그램 종료
	remove()	//file remove
[Given]		: Nothing.
[Returns]	: Nothing.
*/
void
SigIntHandler(int signo)
{
	//remove fifo file
	if (remove(SERV_FIFO) < 0) {
		perror("remove");
		exit(1);
	}

	exit(0);
}

/*
[Program Name]  : fifo server program
[Description]   : fifo로 client로부터 요청받고 응답하는 server program
[Input]		: Nothing.
[Output]	: 어떤 client로부터 요청받았는지 출력
[Calls]         :
	signal()	//signal handler 등록
	exit()		//프로세스 종료
	perror()	//에러 메세지 출력
	mkfifo()	//fifo file 생성
	printf()	//메세지 출력
	close()		//file descriptor close
	open()		//file open, return file descriptor
	read()		//file에서 데이터를 read
	sprintf()	//버퍼에 문자열 저장
	write()		//file에 데이터 write
*/
int
main()
{
	int			fd, cfd, n;
	MsgType		msg;

	//SIGINT signal handler 등록
	if (signal(SIGINT, SigIntHandler) == SIG_ERR) {
		perror("signal");
		exit(1);
	}

	//fifo.h에 정의된 SERV_FIFO 이름으로 fifo 생성
	//0600 : 나만 읽고 쓰기
	if (mkfifo(SERV_FIFO, 0600) < 0) {
		//이미 해당 이름의 fifo가 있는 것은 에러가 아님
		if (errno != EEXIST) {//실패하면 프로세스 종료
			perror("mkfifo");
			exit(1);
		}
	}

	//fifo file open
	if ((fd = open(SERV_FIFO, O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	while (1) {
		//client로부터 메세지를 기다림
		if ((n = read(fd, (char*)&msg, sizeof(msg))) < 0) {
			if (errno == EINTR) {//시스템 콜 수행 중 interrupt가 걸려 중단된 경우
				continue;
			}
			else {
				perror("read");
				exit(1);
			}
		}
		printf("Received request: %s.....", msg.data);

		//clinet에게 응답하기 위한 fifo file open
		if ((cfd = open(msg.returnFifo, O_WRONLY)) < 0) {
			perror("open");
			exit(1);
		}
		//msg.data에 문자열 저장
		sprintf(msg.data, "This is a reply from %d.", getpid());
		//fifo에 msg를 작성함으로서 clinet에게 메세지 전달
		write(cfd, (char*)&msg, sizeof(msg));
		//fifo close
		close(cfd);
		printf("Replied.\n");
	}
}
