#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


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
	char	data[128];
	FILE* ffd;

	//fifo.h에 정의된 SERV_FIFO 이름으로 fifo 생성
	//0600 : 나만 읽고 쓰기
	if (mkfifo("./.myfifo", 0600) < 0) {
		//이미 해당 이름의 fifo가 있는 것은 에러가 아님
		if (errno != EEXIST) {//실패하면 프로세스 종료
			perror("mkfifo");
			exit(1);
		}
	}

	//fifo file open
	if ((fd = open("./.myfifo", O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	while (1) {
		//client로부터 메세지를 기다림
		if ((n = read(fd, data, sizeof(data))) < 0) {
			if (errno == EINTR) {//시스템 콜 수행 중 interrupt가 걸려 중단된 경우
				continue;
			}
			else {
				perror("read");
				exit(1);
			}
		}

		char* ptr = strtok(data, " ");      // " " 공백 문자를 기준으로 문자열을 자름, 포인터 반환
		char fname[128];
		sprintf(fname, ptr);
		strcat(fname, ".txt");
		if ((ffd = fopen(fname, "at")) == NULL) {//argv[2] text file open에 실패하면 프로그램 종료
			perror("fopen");
			exit(1);
		}
		char str[128];
		while (ptr != NULL)               // 자른 문자열이 나오지 않을 때까지 반복
		{
			ptr = strtok(NULL, " ");      // 다음 문자열을 잘라서 포인터를 반환
			sprintf(str, ptr);
			strcat(str, " ");
			fwrite(str, sizeof(char), strlen(str), ffd);
		}
		fwrite(" ", sizeof(char), strlen(" "), ffd);
		//fifo close
		close(fd);
		break;
	}
	close(ffd);
	//remove fifo file
	if (remove("./.myfifo") < 0) {
		perror("remove");
		exit(1);
	}

}
