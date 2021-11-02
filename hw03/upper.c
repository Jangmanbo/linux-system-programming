#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : text file upper program
[Description]   : argv[1] ������ ��� �빮�ڷ� �����Ͽ� ����ϴ� ���α׷�
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, �빮�ڷ� �ٲپ� ����� ����
[Output]        : Nothing.
[Calls]         : fprintf, fopen, fgetc, printf, fclose
*/
int
main(int argc, char* argv[])
{
	FILE* src, * dst;
	int		ch;

	if (argc != 2) {//argv!=2�̸� ���α׷� ����
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//rt:text file�� read
	//fopen : argv[1] �ؽ�Ʈ ������ open, ������ ����Ű�� �����͸� return
	if ((src = fopen(argv[1], "rt")) == NULL) {//argv[1] text file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		exit(1);
	}

	//fgetc : src file pointer�� �о�� char�� return, ������ ������ ������ EOF�� return
	//������ ������ ������ ���� ��������
	while ((ch = fgetc(src)) != EOF) {
		if (ch >= 97 && ch <= 122) { //char�� �ҹ��ڸ� �빮�ڷ� ����
			ch -= 32;
		}
		printf("%c", ch); //char ������ ���
	}

	fclose(src);
}
