#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

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
main(int argc, char* argv[])
{
	char	fname[128];
	int			fd, sfd, n;
	char	data[128] = "";


	//읽기, 쓰기용으로 file open 
	//server에게 요청할 file descriptor를 return
	if ((sfd = open("./.myfifo", O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	if (argc == 2) {
		char command[128] = "";
		strcat(command, "cat ");
		strcat(command, argv[1]);
		strcat(command, ".txt");
		system(command);

		exit(0);
	}
	for (int i = 1; i < argc; i++) {
		strcat(data, argv[i]);
		strcat(data, " ");
	}

	//msg.data에 문자열 저장
	//sprintf(data, "Hello");
	//sfd file descriptor를 통해 server에 메세지 전달
	write(sfd, data, sizeof(data));

	//close file descriptor
	close(sfd);
}
