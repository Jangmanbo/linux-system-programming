#include <stdio.h>
#include <stdlib.h>

#define	MAX_BUF	1024

/*
[Program Name]  : binary file merge program
[Description]   : argv[1], argv[2] 파일을 argv[3]파일로 merge
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, merge할 파일-1,2, merge하여 새로 생성된 파일
[Output]        : merge하여 새로 생성된 파일
[Calls]         : fprintf, fopen, fread, fwrite, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src1, * src2, * dst;
	char	buf[MAX_BUF];
	int		count;

	if (argc != 4) {//argc!=4면 프로그램 종료
		fprintf(stderr, "Usage: %s source1 source2 dest\n", argv[0]);
		exit(1);
	}

	//rb : binary file을 read
		//fopen : argv[1]을 가리키는 file pointer를 리턴(open에 실패하면 NULL return)
	if ((src1 = fopen(argv[1], "rb")) == NULL) {//argv[1] file open에 실패하면 프로그램 종료
		perror("fopen");
		exit(1);
	}

	//rb : binary file을 read
		//fopen : argv[2]을 가리키는 file pointer를 리턴(open에 실패하면 NULL return)
	if ((src2 = fopen(argv[2], "rb")) == NULL) {//argv[2] file open에 실패하면 프로그램 종료
		perror("fopen");
		exit(1);
	}

	//wb : binary file을 write
		//fopen : argv[3]을 가리키는 file pointer를 리턴(open에 실패하면 NULL return)
	if ((dst = fopen(argv[3], "wb")) == NULL) {//argv[3] file open에 실패하면 프로그램 종료
		perror("fopen");
		exit(1);
	}

	//src1에서 1byte 단위로 최대 MAX_BUF번 read해서 buf배열에 저장
		//파일을 모두 읽으면 0을 return 하여 while loop을 빠져나옴	
	while ((count = fread(buf, 1, MAX_BUF, src1)) > 0) {
		//buf로부터 1byte씩 count번 dst에 write
		fwrite(buf, 1, count, dst);
	}

	//src2에서 1byte 단위로 최대 MAX_BUF번 read해서 buf배열에 저장
		//파일을 모두 읽으면 0을 return 하여 while loop을 빠져나옴
	while ((count = fread(buf, 1, MAX_BUF, src2)) > 0) {
		//buf로부터 1byte씩 count번 dst에 write
		fwrite(buf, 1, count, dst);
	}

	fclose(src1);
	fclose(src2);
	fclose(dst);
}
