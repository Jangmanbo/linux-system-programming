#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "fifo.h"


/*
[Program Name]  : fifo clinet program
[Description]   : fifo로 server에게 요청하고 응답받는 client  program
[Input]         : Nothing.
[Output]        : 서버에게 응답받은 메세지 출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		mkfifo()        //fifo file 생성
		printf()        //메세지 출력
		close()         //file descriptor close
		open()          //file open, return file descriptor
		read()          //file에서 데이터를 read
		sprintf()       //배열에 문자열 저장
		write()         //file에 데이터 write
	strcpy()	//문자열 목사
	remove()	//file remove
*/
int
main()
{
	char		fname[MAX_FIFO_NAME];
	int			fd, sfd, n;
	MsgType		msg;

	//clinet specific fifo 필요
	//fname에 .fifo에 processid를 붙인 문자열 저장
	sprintf(fname, ".fifo%d", getpid());

	//fifo file 생성
	//0600 : 나만 읽고 쓰기
	if (mkfifo(fname, 0600) < 0) {
		perror("mkfifo");
		exit(1);
	}

	//읽기, 쓰기용으로 file open 
	//server로부터 응답받을 file descriptor를 return
	if ((fd = open(fname, O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}
	//읽기, 쓰기용으로 file open 
	//server에게 요청할 file descriptor를 return
	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	//client specific fifo의 이름을 msg.returnFifo에 복사하여 저장
	strcpy(msg.returnFifo, fname);
	//msg.data에 문자열 저장
	sprintf(msg.data, "This is a request from %d.", getpid());
	//sfd file descriptor를 통해 server에 메세지 전달
	write(sfd, (char*)&msg, sizeof(msg));
	printf("Sent a request.....");

	//client specific fifo file descriptor를 통해 server로부터 메세지를 read
	if ((n = read(fd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);

	//close file descriptor
	close(fd);
	close(sfd);

	//remove client specific fifo file
	if (remove(fname) < 0) {
		perror("remove");
		exit(1);
	}
}
