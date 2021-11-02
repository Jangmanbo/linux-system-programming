#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define	MAX_BUF	1024

/*
[Function Name] : file size
[Description]	: file의 size를 계산
[Input]
		FILE *fp	//size를 계산할 파일의 포인터
[Output]
	long size	//file의 크기
[Call By]	: main()
[Calls]         : ftell, fseek, ftell
[Given]		: Nothing
[Returns]	: Nothing
*/
long
filesize(FILE* fp)
{
	long	cur, size;

	//현재 file pointer의 위치를 저장
	//이 프로그램에서 filesize함수를 파일을 open한 직후 사용
	//즉 file pointer는 현재 파일의 처음에 있음	
	cur = ftell(fp);

	//file pointer를 파일의 끝으로 보냄
	fseek(fp, 0L, SEEK_END);

	//file pointer의 현재 위치를 return, 즉 파일의 크기
	size = ftell(fp);

	//file pointer를 원래 위치로 돌려보냄
	fseek(fp, cur, SEEK_SET);

	//file의 size를 return
	return(size);
}

/*
[Program Name]  : binary file split program
[Description]   : argv[1] 파일을 argv[2], argv[3] 파일로 나눔
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, split할 파일, split하여 새로 생성된 파일-1, 파일-2
[Output]        : split하여 새로 생성된 2개의 binary file
[Calls]         : fseek, fprintf, fopen, fread, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst1, * dst2;
	char	buf[MAX_BUF];
	int		count, size;

	if (argc != 4) {//argv!=4면 프로그램 종료
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
		exit(1);
	}

	//rb : binary file을 read
		//fopen : argv[1]을 가리키는 file pointer를 리턴(open에 실패하면 NULL return)
	if ((src = fopen(argv[1], "rb")) == NULL) {//argv[1] file open에 실패하면 프로그램 종료
		perror("fopen");
		exit(1);
	}

	//wb : binary file을 write
		//fopen : argv[2]을 가리키는 file pointer를 리턴(open에 실패하면 NULL return)
	if ((dst1 = fopen(argv[2], "wb")) == NULL) {//argv[2] file open에 실패하면 프로그램 종료
		perror("fopen");
		exit(1);
	}

	//wb : binary file을 write
		//fopen : argv[3]을 가리키는 file pointer를 리턴(open에 실패하면 NULL return)
	if ((dst2 = fopen(argv[3], "wb")) == NULL) {//argv[3] file open에 실패하면 프로그램 종료
		perror("fopen");
		exit(1);
	}

	//나누려는 file의 size를2로 나누어 size variable에 저장
	size = filesize(src) / 2;

	//src로부터 최대 MAX_BUF만큼 읽고 dst1에 write
	//파일을 읽은 만큼 size 값을 차감하다가 0이 되면, 즉 파일을 다 읽으면 루프를 빠져나옴
	while (size > 0) {
		count = (size > MAX_BUF) ? MAX_BUF : size;
		fread(buf, 1, count, src);
		fwrite(buf, 1, count, dst1);
		size -= count;
	}

	//src로부터 최대 MAX_BUF만큼 읽고 dst2에 write
	//파일의 끝까지 다 읽으면 fread가 0을 리턴하여 루프를 빠져나옴
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0) {
		fwrite(buf, 1, count, dst2);
	}

	fclose(src);
	fclose(dst1);
	fclose(dst2);
}
