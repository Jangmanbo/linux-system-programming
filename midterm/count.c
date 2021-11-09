#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char* argv[])
{
	FILE* src;
	char ch;
	int count = 0;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//argv[1]파일을 text read 모드로 open
	if ((src = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	}

	printf("%c\n", *argv[2]);

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
