#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "record.h"

/*
[Program Name]  : text file convert program
[Description]   : argv[1] text file을 argv[2] recode 단위의 파일로 convert
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, text file, record 단위로 convert할 파일
[Output]        : record 단위로 write된 파일
[Calls]         : fprintf, fopen, fgets, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	Record	rec;

	if (argc != 3) {//argc!=3이면 프로그램 종료
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}

	//rt : text file을 read
		//fopen : argv[1]을 가리키는 file pointer를 리턴(open에 실패하면 NULL return)
	if ((src = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	}
	//wb : binary file을 write
		//fopen : argv[2]을 가리키는 file pointer를 리턴(open에 실패하면 NULL return)
	if ((dst = fopen(argv[2], "wb")) == NULL) {
		perror("fopen");
		exit(1);
	}

	//fgets: file pointer를 통해 읽은 문자열을 rec.stud배열에 한 줄씩 저장(최대 SMAX)
		//파일의 끝까지 읽었다면 null pointer(=0)을 return하여 루프 빠져나옴
	while (fgets(rec.stud, SMAX, src)) {
		*strchr(rec.stud, '\n') = '\0';	//문자열의 끝을 나타내는 '\n'을 '\0'으로 바꿈
		fgets(rec.num, NMAX, src);	//file pointer를 통해 읽은 문자열을 rec.num배열에 한 줄씩 저장(최대 NMAX)
		*strchr(rec.num, '\n') = '\0';	//문자열의 끝을 나타내는 '\n'을 '\0'으로 바꿈
		fgets(rec.dept, DMAX, src);	//file pointer를 통해 읽은 문자열을 rec.dept배열에 한 줄씩 저장(최대 DMAX)
		*strchr(rec.dept, '\n') = '\0';	//문자열의 끝을 나타내는 '\n'을 '\0'으로 바꿈

		//record 단위로 read한 것을 dst file pointer를 통해 파일에 저장
		fwrite(&rec, sizeof(rec), 1, dst);
	}

	fclose(src);
	fclose(dst);
}
