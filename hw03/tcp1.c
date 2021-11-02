#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : text file copy program 1
[Description]   : argv[1] 파일을 argv[2]파일로 copy
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, 복사할 파일, 복사하여 새로 만들 파일
[Output]        : 복사하여 새로 만든 텍스트 파일
[Calls]         : fprintf, fopen, fgetc, fputc, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	int		ch;

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

	//fgetc : src file pointer로 읽어온 char를 return, 파일의 끝까지 읽으면 EOF를 return
	//파일의 끝까지 읽으면 루프 빠져나옴
	while ((ch = fgetc(src)) != EOF) {
		//ch를 dst file pointer를 통해 파일에 write
		fputc(ch, dst);
	}

	fclose(src);
	fclose(dst);
}
