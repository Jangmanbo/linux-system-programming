#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : text file copy program 1
[Description]   : argv[1] ������ argv[2]���Ϸ� copy
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, ������ ����, �����Ͽ� ���� ���� ����
[Output]        : �����Ͽ� ���� ���� �ؽ�Ʈ ����
[Calls]         : fprintf, fopen, fgetc, fputc, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	int		ch;

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

	//fgetc : src file pointer�� �о�� char�� return, ������ ������ ������ EOF�� return
	//������ ������ ������ ���� ��������
	while ((ch = fgetc(src)) != EOF) {
		//ch�� dst file pointer�� ���� ���Ͽ� write
		fputc(ch, dst);
	}

	fclose(src);
	fclose(dst);
}
