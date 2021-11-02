#include <stdio.h>
#include <stdlib.h>

#define	MAX_BUF	256

/*
[Program Name]  : text file copy program 2
[Description]   : argv[1] ������ argv[2]���Ϸ� copy
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, ������ ����, �����Ͽ� ���� ���� ����
[Output]        : �����Ͽ� ���� ���� �ؽ�Ʈ ����
[Calls]         : fprintf, fopen, fgets, fputc, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	char	buf[MAX_BUF];

	if (argc != 3) {//argv!=3�̸� ���α׷� ����
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//rt:text file�� read
		//fopen : argv[1] �ؽ�Ʈ ������ open, ������ ����Ű�� �����͸� return
	if ((src = fopen(argv[1], "rt")) == NULL) {//argv[1] text file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		exit(1);
	}

	//wt:text file�� write
		//fopen : argv[2] �ؽ�Ʈ ������ open, ������ ����Ű�� �����͸� return
	if ((dst = fopen(argv[2], "wt")) == NULL) {//argv[2] text file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		exit(1);
	}

	//fgets: file pointer�� ���� ���� ���ڿ��� buf�� �� �پ� ����
		//������ ������ �о��ٸ� NULL�� return�Ͽ� ���� ��������
	while (fgets(buf, MAX_BUF, src) != NULL) {
		//dst file pointer�� ���� buf�� ����� ���ڿ��� ���Ͽ� write
		fputs(buf, dst);
	}

	fclose(src);
	fclose(dst);
}
