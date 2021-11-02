#include <stdio.h>
#include <stdlib.h>

#define	MAX_BUF	1024

/*
[Program Name]  : binary file copy program
[Description]   : argv[1] 파일을 argv[2]파일로 copy
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, 복사할 파일, 복사하여 새로 만들 파일의 포인터
[Output]        : 복사하여 새로 만든 binary file
[Calls]         : fprintf, fopen, fread, fwrite, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	char	buf[MAX_BUF];
	int		count;

	if (argc != 3) {//argv!=3이면 프로그램 종료
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
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
	if ((dst = fopen(argv[2], "wb")) == NULL) {//argv[2] file open에 실패하면 프로그램 종
		perror("fopen");
		exit(1);
	}

	//src에서 1byte 단위로 최대 MAX_BUF번 read해서 buf배열에 저장
	//파일을 모두 읽으면 0을 return 하여 while loop을 빠져나옴
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0) {
		//buf로부터 1byte씩 count번 dst에 write
		fwrite(buf, 1, count, dst);
	}

	fclose(src);
	fclose(dst);
}
