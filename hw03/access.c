#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"

/*
[Function Name] : print record
[Description]   : record 단위로 출력
[Input]
		Record *rp	//record pointer
[Output]	: Nothing
[Call By]       : main()
[Calls]         : Nothing
[Given]         : Nothing
[Returns]       : Nothing
*/
void
print_record(Record* rp)
{
	puts(rp->stud);
	puts(rp->num);
	puts(rp->dept);
}

/*
[Program Name]  : record file access program
[Description]   : argv[1] record file을 read/write하는 프로그램
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, read/write할 record file
[Output]        : Nothing.
[Calls]         : fprintf, fopen, fread, fseek, fwrite, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* fp;
	Record	rec;

	if (argc != 2) {//argc!=2면 프로그램 종료
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}

	//r+b : read모드로 binary file open
	//fopen : argv[1]을 가리키는 file pointer를 리턴(open에 실패하면 NULL return)
	if ((fp = fopen(argv[1], "r+b")) == NULL) {
		perror("fopen");
		exit(1);
	}

	printf("-----Ordered Record List-----\n");

	//record 단위로 file을 read해서 record 구조체에 저장
		//파일을 모두 읽으면 0을 return 하여 while loop을 빠져나옴
	while (fread(&rec, sizeof(rec), 1, fp)) {
		//record 단위로 출력
		print_record(&rec);
	}
	rewind(fp); //file pointer의 위치를 0으로 설정
	getchar(); //중간중간 프로그램을 멈춰서 결과 확인

	printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n");

	fseek(fp, sizeof(rec) * 2L, SEEK_SET);	//file pointer를 3번째 record로 이동
	fread(&rec, sizeof(rec), 1, fp);	//record 단위로 file을 read해서 record 구조체에 저장
	print_record(&rec);	//record 단위로 출력

	fseek(fp, sizeof(rec) * 5L, SEEK_SET);	//file pointer를 6번째 record로 이동
	fread(&rec, sizeof(rec), 1, fp);	//record 단위로 file을 read해서 record 구조체에 저장
	print_record(&rec);	//record 단위로 출력

	fseek(fp, sizeof(rec) * 1L, SEEK_SET);	//file pointer를 2번째 record로 이동
	fread(&rec, sizeof(rec), 1, fp);	//record 단위로 file을 read해서 record 구조체에 저장
	print_record(&rec);	//record 단위로 출력

	fseek(fp, sizeof(rec) * 3L, SEEK_SET);	//file pointer를4번째 record로 이동
	fread(&rec, sizeof(rec), 1, fp);	//record 단위로 file을 read해서 record 구조체에 저장
	print_record(&rec);	//record 단위로 출력

	fseek(fp, sizeof(rec) * 0L, SEEK_SET);	//file pointer를1번째 record로 이동
	fread(&rec, sizeof(rec), 1, fp);	//record 단위로 file을 read해서 record 구조체에 저장
	print_record(&rec);	//record 단위로 출력

	fseek(fp, sizeof(rec) * 4L, SEEK_SET);	//file pointer를5번째 record로 이동
	fread(&rec, sizeof(rec), 1, fp);	//record 단위로 file을 read해서 record 구조체에 저장
	print_record(&rec);	//record 단위로 출력
	getchar();	//잠시 프로그램 중단하여 결과 확인

	printf("-----Updated Record List-----\n");

	fseek(fp, sizeof(rec) * 2L, SEEK_SET);	//file pointer를 3번째 record로 이동
	fread(&rec, sizeof(rec), 1, fp);	//record 단위로 file을 read해서 record 구조체에 저장
	strcpy(rec.dept, "Movie");		//rec.dept에 "Movie" 문자열 복사(record 구조체의 정보 수정)
	fseek(fp, sizeof(rec) * 2L, SEEK_SET);	//file pointer를 3번째 record로 이동
	fwrite(&rec, sizeof(rec), 1, fp);	//수정된 record 구조체의 정보를 파일에 저장

	fseek(fp, sizeof(rec) * 5L, SEEK_SET);	//file pointer를 6번째 record로 이동
	fread(&rec, sizeof(rec), 1, fp);	//record 단위로 file을 read해서 record 구조체에 저장
	strcpy(rec.dept, "Baseball");		//rec.dept에 "Baseball" 문자열 복사(record 구조체의 정보 수정)
	fseek(fp, sizeof(rec) * 5L, SEEK_SET);	//file pointer를 6번째 record로 이동
	fwrite(&rec, sizeof(rec), 1, fp);	//수정된 record 구조체의 정보를 파일에 저장

	rewind(fp);	//file pointer의 위치를 0으로 설정

	//record 단위로 file을 read해서 record 구조체에 저장
		//파일을 모두 읽으면 0을 return 하여 while loop을 빠져나옴
	while (fread(&rec, sizeof(rec), 1, fp)) {
		//record 단위로 출력
		print_record(&rec);
	}

	fclose(fp);
}
