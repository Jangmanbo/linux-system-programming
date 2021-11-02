#include <stdio.h>
#include <stdlib.h>

#define	MAX_BUF	256

/*
[Program Name]  : text file copy program 2
[Description]   : argv[1] 파일을 argv[2]파일로 copy
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, 복사할 파일, 복사하여 새로 만들 파일
[Output]        : 복사하여 새로 만든 텍스트 파일
[Calls]         : fprintf, fopen, fgets, fputc, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	char	buf[MAX_BUF];

	if (argc != 3) {//argv!=3이면 프로그램 종료
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//rt:text file을 read
		//fopen : argv[1] 텍스트 파일을 open, 파일을 가리키는 포인터를 return
	if ((src = fopen(argv[1], "rt")) == NULL) {//argv[1] text file open에 실패하면 프로그램 종료
		perror("fopen");
		exit(1);
	}

	//wt:text file을 write
		//fopen : argv[2] 텍스트 파일을 open, 파일을 가리키는 포인터를 return
	if ((dst = fopen(argv[2], "wt")) == NULL) {//argv[2] text file open에 실패하면 프로그램 종료
		perror("fopen");
		exit(1);
	}

	//fgets: file pointer를 통해 읽은 문자열을 buf에 한 줄씩 저장
		//파일의 끝까지 읽었다면 NULL을 return하여 루프 빠져나옴
	while (fgets(buf, MAX_BUF, src) != NULL) {
		//dst file pointer를 통해 buf에 저장된 문자열을 파일에 write
		fputs(buf, dst);
	}

	fclose(src);
	fclose(dst);
}
