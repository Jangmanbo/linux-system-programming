#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "record.h"

/*
[Program Name]  : text file convert program
[Description]   : argv[1] text file�� argv[2] recode ������ ���Ϸ� convert
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, text file, record ������ convert�� ����
[Output]        : record ������ write�� ����
[Calls]         : fprintf, fopen, fgets, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	Record	rec;

	if (argc != 3) {//argc!=3�̸� ���α׷� ����
		fprintf(stderr, "Usage: %s source dest\n", argv[0]);
		exit(1);
	}

	//rt : text file�� read
		//fopen : argv[1]�� ����Ű�� file pointer�� ����(open�� �����ϸ� NULL return)
	if ((src = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	}
	//wb : binary file�� write
		//fopen : argv[2]�� ����Ű�� file pointer�� ����(open�� �����ϸ� NULL return)
	if ((dst = fopen(argv[2], "wb")) == NULL) {
		perror("fopen");
		exit(1);
	}

	//fgets: file pointer�� ���� ���� ���ڿ��� rec.stud�迭�� �� �پ� ����(�ִ� SMAX)
		//������ ������ �о��ٸ� null pointer(=0)�� return�Ͽ� ���� ��������
	while (fgets(rec.stud, SMAX, src)) {
		*strchr(rec.stud, '\n') = '\0';	//���ڿ��� ���� ��Ÿ���� '\n'�� '\0'���� �ٲ�
		fgets(rec.num, NMAX, src);	//file pointer�� ���� ���� ���ڿ��� rec.num�迭�� �� �پ� ����(�ִ� NMAX)
		*strchr(rec.num, '\n') = '\0';	//���ڿ��� ���� ��Ÿ���� '\n'�� '\0'���� �ٲ�
		fgets(rec.dept, DMAX, src);	//file pointer�� ���� ���� ���ڿ��� rec.dept�迭�� �� �پ� ����(�ִ� DMAX)
		*strchr(rec.dept, '\n') = '\0';	//���ڿ��� ���� ��Ÿ���� '\n'�� '\0'���� �ٲ�

		//record ������ read�� ���� dst file pointer�� ���� ���Ͽ� ����
		fwrite(&rec, sizeof(rec), 1, dst);
	}

	fclose(src);
	fclose(dst);
}
