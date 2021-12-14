#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
[Program Name]  : memory-mapped file copy program
[Description]   : memory-mapped file을 통해 file copy
[Input]
	int argc        // argv 배열의 크기
		char *argv[]    // 읽을 파일,  복사하여 새로 생성할 파일
[Output]        : Nothing.
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		fprintf()       //파일에 메세지 출력
	open()		//open file
	fstat()		//열린 file의 status를 읽고 성공여부를 return
	lseek()		//file curosr 조정
	write()		//file에 데이터 write
	mmap()		//virtual address와 file을 mapping
	memcpy()	//메모리의 값을 복사하여 붙여넣기
	close()		//close file
*/
int
main(int argc, char* argv[])
{
	int			fdin, fdout;
	char* src, * dst;
	struct stat	statbuf;

	if (argc != 3) {//argc!=3이면 에러 메세지 출력 후 프로세스 종료
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//데이터를 읽어올 파일을 읽기모드로 open
	if ((fdin = open(argv[1], O_RDONLY)) < 0) {
		perror("open");
		exit(1);
	}
	//복사한 내용을 붙여넣을 파일 open
	//없으면 새로 생성, 있으면 내용을 모두 삭제
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
		perror("open");
		exit(1);
	}

	//fdin 파일의 status를 statbuf에 저장
	if (fstat(fdin, &statbuf) < 0) {
		perror("fstat");
		exit(1);
	}

	//file cursor를 파일의 마지막까지 이동
	if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) < 0) {
		perror("lseek");
		exit(1);
	}
	//파일의 마지막에 ""작성
	write(fdout, "", 1);
	//=>즉 size가 st_size인 0으로 채워진 더미 파일 생성

	//offset=0, size=statbubf.st_size이므로
	//파일의 처음부터 끝까지 읽어 virtual memory에 mapping
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0))
		== (caddr_t)-1) {
		perror("mmap");
		exit(1);
	}
	//fdout파일을 fdin파일의 크기만큼 virtual memory에 mapping
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0))
		== (caddr_t)-1) {
		perror("mmap");
		exit(1);
	}

	//메모리 값을 복사
	memcpy(dst, src, statbuf.st_size);

	//close file descriptor
	close(fdin);
	close(fdout);
}
