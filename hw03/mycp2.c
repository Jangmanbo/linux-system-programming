#include <stdio.h>
#include <stdlib.h>

#define	MAX_BUF	1024

/*
[Program Name]  : binary file copy program
[Description]   : argv[1] ������ argv[2]���Ϸ� copy
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, ������ ����, �����Ͽ� ���� ���� ������ ������
[Output]        : �����Ͽ� ���� ���� binary file
[Calls]         : fprintf, fopen, fread, fwrite, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	char	buf[MAX_BUF];
	int		count;

	if (argc != 3) {//argv!=3�̸� ���α׷� ����
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//rb : binary file�� read
	//fopen : argv[1]�� ����Ű�� file pointer�� ����(open�� �����ϸ� NULL return)	
	if ((src = fopen(argv[1], "rb")) == NULL) {//argv[1] file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		exit(1);
	}

	//wb : binary file�� write
		//fopen : argv[2]�� ����Ű�� file pointer�� ����(open�� �����ϸ� NULL return)
	if ((dst = fopen(argv[2], "wb")) == NULL) {//argv[2] file open�� �����ϸ� ���α׷� ��
		perror("fopen");
		exit(1);
	}

	//src���� 1byte ������ �ִ� MAX_BUF�� read�ؼ� buf�迭�� ����
	//������ ��� ������ 0�� return �Ͽ� while loop�� ��������
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0) {
		//buf�κ��� 1byte�� count�� dst�� write
		fwrite(buf, 1, count, dst);
	}

	fclose(src);
	fclose(dst);
}
