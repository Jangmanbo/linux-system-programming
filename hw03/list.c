#include <stdio.h>
#include <stdlib.h>

#define	MAX_BUF	256

/*
[Program Name]  : line numper program
[Description]   : 파일을 line number와 함께 출력
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, 출력할 파일
[Output]        : line number와 함께 파일의 내용을 출력
[Calls]         : printf, fopen, fgets, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* fp;
	char	buf[MAX_BUF];
	int		line;

	if (argc != 2) {//argv의크기가 2가 아니면 프로그램 종료
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	//rt:text file을 read
	//fopen은 argv[1]을 가리키는 file pointer를 리턴
	if ((fp = fopen(argv[1], "rt")) == NULL) {//argv[1]파일 open에 실패
		perror("fopen");
		exit(1);
	}

	line = 1;
	//fgets: file pointer를 통해 읽은 문자열을 buf에 한 줄씩 저장
	//파일의 끝까지 읽었다면 null pointer(=0)을 return하여 루프 빠져나옴
	while (fgets(buf, MAX_BUF, fp)) {
		//line을 먼저 출력하고 buf  출력(line 1 증가)
		printf("%4d: %s", line++, buf);
	}

	fclose(fp);
}
