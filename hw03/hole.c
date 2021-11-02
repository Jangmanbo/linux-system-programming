#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : file create program
[Description]   : file.hole이라는 1024byte 더미 파일 만들기 프로그램
[Input]		: Nothing.
[Output]        : 새로 만든 1024byte 더미 파일(file.hole)
[Calls]         : creat, lseek, write, close
*/
int
main()
{
	int 	fd;
	char	ch = '\0';

	//file.hole이라는 비어있는 파일 생성
	//0400:소유자 읽기 권한
	if ((fd = creat("file.hole", 0400)) < 0) {
		perror("creat");
		exit(1);
	}

	//file pointer를 파일의 가장 앞에서부터 1023byte 이동
	if (lseek(fd, 1023, SEEK_SET) < 0) {
		perror("lseek");
		exit(1);
	}
	write(fd, &ch, 1); //마지막 1byte에 '\0'을 write

	close(fd);
}
