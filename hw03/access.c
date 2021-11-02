#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"

/*
[Function Name] : print record
[Description]   : record ������ ���
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
[Description]   : argv[1] record file�� read/write�ϴ� ���α׷�
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, read/write�� record file
[Output]        : Nothing.
[Calls]         : fprintf, fopen, fread, fseek, fwrite, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* fp;
	Record	rec;

	if (argc != 2) {//argc!=2�� ���α׷� ����
		fprintf(stderr, "Usage: %s file\n", argv[0]);
		exit(1);
	}

	//r+b : read���� binary file open
	//fopen : argv[1]�� ����Ű�� file pointer�� ����(open�� �����ϸ� NULL return)
	if ((fp = fopen(argv[1], "r+b")) == NULL) {
		perror("fopen");
		exit(1);
	}

	printf("-----Ordered Record List-----\n");

	//record ������ file�� read�ؼ� record ����ü�� ����
		//������ ��� ������ 0�� return �Ͽ� while loop�� ��������
	while (fread(&rec, sizeof(rec), 1, fp)) {
		//record ������ ���
		print_record(&rec);
	}
	rewind(fp); //file pointer�� ��ġ�� 0���� ����
	getchar(); //�߰��߰� ���α׷��� ���缭 ��� Ȯ��

	printf("-----Shuffled Record List (3, 6, 2, 4, 1, 5)-----\n");

	fseek(fp, sizeof(rec) * 2L, SEEK_SET);	//file pointer�� 3��° record�� �̵�
	fread(&rec, sizeof(rec), 1, fp);	//record ������ file�� read�ؼ� record ����ü�� ����
	print_record(&rec);	//record ������ ���

	fseek(fp, sizeof(rec) * 5L, SEEK_SET);	//file pointer�� 6��° record�� �̵�
	fread(&rec, sizeof(rec), 1, fp);	//record ������ file�� read�ؼ� record ����ü�� ����
	print_record(&rec);	//record ������ ���

	fseek(fp, sizeof(rec) * 1L, SEEK_SET);	//file pointer�� 2��° record�� �̵�
	fread(&rec, sizeof(rec), 1, fp);	//record ������ file�� read�ؼ� record ����ü�� ����
	print_record(&rec);	//record ������ ���

	fseek(fp, sizeof(rec) * 3L, SEEK_SET);	//file pointer��4��° record�� �̵�
	fread(&rec, sizeof(rec), 1, fp);	//record ������ file�� read�ؼ� record ����ü�� ����
	print_record(&rec);	//record ������ ���

	fseek(fp, sizeof(rec) * 0L, SEEK_SET);	//file pointer��1��° record�� �̵�
	fread(&rec, sizeof(rec), 1, fp);	//record ������ file�� read�ؼ� record ����ü�� ����
	print_record(&rec);	//record ������ ���

	fseek(fp, sizeof(rec) * 4L, SEEK_SET);	//file pointer��5��° record�� �̵�
	fread(&rec, sizeof(rec), 1, fp);	//record ������ file�� read�ؼ� record ����ü�� ����
	print_record(&rec);	//record ������ ���
	getchar();	//��� ���α׷� �ߴ��Ͽ� ��� Ȯ��

	printf("-----Updated Record List-----\n");

	fseek(fp, sizeof(rec) * 2L, SEEK_SET);	//file pointer�� 3��° record�� �̵�
	fread(&rec, sizeof(rec), 1, fp);	//record ������ file�� read�ؼ� record ����ü�� ����
	strcpy(rec.dept, "Movie");		//rec.dept�� "Movie" ���ڿ� ����(record ����ü�� ���� ����)
	fseek(fp, sizeof(rec) * 2L, SEEK_SET);	//file pointer�� 3��° record�� �̵�
	fwrite(&rec, sizeof(rec), 1, fp);	//������ record ����ü�� ������ ���Ͽ� ����

	fseek(fp, sizeof(rec) * 5L, SEEK_SET);	//file pointer�� 6��° record�� �̵�
	fread(&rec, sizeof(rec), 1, fp);	//record ������ file�� read�ؼ� record ����ü�� ����
	strcpy(rec.dept, "Baseball");		//rec.dept�� "Baseball" ���ڿ� ����(record ����ü�� ���� ����)
	fseek(fp, sizeof(rec) * 5L, SEEK_SET);	//file pointer�� 6��° record�� �̵�
	fwrite(&rec, sizeof(rec), 1, fp);	//������ record ����ü�� ������ ���Ͽ� ����

	rewind(fp);	//file pointer�� ��ġ�� 0���� ����

	//record ������ file�� read�ؼ� record ����ü�� ����
		//������ ��� ������ 0�� return �Ͽ� while loop�� ��������
	while (fread(&rec, sizeof(rec), 1, fp)) {
		//record ������ ���
		print_record(&rec);
	}

	fclose(fp);
}
