#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : text file upper program
[Description]   : argv[1] 파일을 모두 대문자로 변경하여 출력하는 프로그램
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, 대문자로 바꾸어 출력할 파일
[Output]        : Nothing.
[Calls]         : fprintf, fopen, fgetc, printf, fclose
*/
int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	int		ch;

	if (argc != 2) {//argv!=2이면 프로그램 종료
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//rt:text file을 read
	//fopen : argv[1] 텍스트 파일을 open, 파일을 가리키는 포인터를 return
	if ((src = fopen(argv[1], "rt")) == NULL) {//argv[1] text file open에 실패하면 프로그램 종료
		perror("fopen");
		exit(1);
	}

	//fgetc : src file pointer로 읽어온 char를 return, 파일의 끝까지 읽으면 EOF를 return
	//파일의 끝까지 읽으면 루프 빠져나옴
	while ((ch = fgetc(src)) != EOF) {
		if (ch >= 97 && ch <= 122) { //char이 소문자면 대문자로 변경
			ch -= 32;
		}
		printf("%c", ch); //char 단위로 출력
	}

	fclose(src);
}
