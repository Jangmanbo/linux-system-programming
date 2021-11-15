#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : char count program
[Description]   : argv[1] 파일에argv[2]가 몇 개 있는지 세는 프로그램
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 파일과 문자
[Output]        : argv[1] 파일에argv[2]가 몇 개 있는지
[Calls]         :
	fprintf()	//파일에 메세지 출력
	exit()		//프로세스 종료
	perror()	//에러 메세지 출력
	printf()	//메세지 출력
	fgetc()		//파일에서 한 글자씩 읽어옴
	fopen()		//file open
	fclose()	//file close
*/

int
main(int argc, char* argv[])
{
	FILE* src;
	char ch;
	int count = 0;

	if (argc != 3) {//argv!=3이면 프로그램 종료
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//argv[1]파일을 text read 모드로 open
	if ((src = fopen(argv[1], "rt")) == NULL) {
		//실패하면 프로세스 종료
		perror("fopen");
		exit(1);
	}


	//argv[1]파일을 한글자씩 읽어옴, 다 읽으면 loop 빠져나옴
	while ((ch = fgetc(src)) != EOF) {
		//읽어온 char이 argv[2]와 일치하면 count 1 증가
		if (ch == *argv[2]) {
			count++;
		}
	}

	//argv[1]파일 내의 argv[2]의 개수 출력
	printf("%d\n", count);
	fclose(src);
}
