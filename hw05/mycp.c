#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define	MAX_BUF	1024

/*
[Program Name]	: copy program
[Description]	: argv[1] 파일을 argv[2]파일로 copy
[Input]
	int argc	// argv 배열의 크기
	char *argv[] 	// 실행할 프로그램, 복사할 파일, 복사하여 새로 만들 파일의 포인터
[Output]	: 복사하여 새로 만든 파일
[Calls]		: printf, open, read, write, close
*/

int
main(int argc, char* argv[])
{
	int 	fd1, fd2, count;
	char	buf[MAX_BUF];

	if (argc != 3) {//argv!=3이면 프로그램 종료
		printf("Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//argv[1]파일을 복사할 것이므로 read only
	if ((fd1 = open(argv[1], O_RDONLY)) < 0) {//argv[1]파일을 읽기에 실패
		perror("open");
		exit(1);
	}

	/*	argv[2]파일에 복사할 것이므로
		O_WRONLY:파일에 쓰기
		O_CREAT:파일 생성
		O_TRUNC:argv[2]파일이 이미 있었다면 기존에 있던 내용 삭제
		0644 : 소유자는 읽기와 쓰기, 그룹과 기타 사용자는 읽기 권한*/
	if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {//argv[2]파일에 복사하기 실패
		perror("open");
		exit(1);
	}

	while (count = read(fd1, buf, MAX_BUF)) {//fd1을 다 읽으면count=0으로 루프 빠져나옴
		write(fd2, buf, count);//buf의 내용을 fd2로 count byte만큼write
	}

	close(fd1);
	close(fd2);
}
