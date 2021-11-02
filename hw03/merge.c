#include <stdio.h>
#include <stdlib.h>

#define	MAX_BUF	1024

/*
[Program Name]  : binary file merge program
[Description]   : argv[1], argv[2] ������ argv[3]���Ϸ� merge
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, merge�� ����-1,2, merge�Ͽ� ���� ������ ����
[Output]        : merge�Ͽ� ���� ������ ����
[Calls]         : fprintf, fopen, fread, fwrite, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src1, * src2, * dst;
	char	buf[MAX_BUF];
	int		count;

	if (argc != 4) {//argc!=4�� ���α׷� ����
		fprintf(stderr, "Usage: %s source1 source2 dest\n", argv[0]);
		exit(1);
	}

	//rb : binary file�� read
		//fopen : argv[1]�� ����Ű�� file pointer�� ����(open�� �����ϸ� NULL return)
	if ((src1 = fopen(argv[1], "rb")) == NULL) {//argv[1] file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		exit(1);
	}

	//rb : binary file�� read
		//fopen : argv[2]�� ����Ű�� file pointer�� ����(open�� �����ϸ� NULL return)
	if ((src2 = fopen(argv[2], "rb")) == NULL) {//argv[2] file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		exit(1);
	}

	//wb : binary file�� write
		//fopen : argv[3]�� ����Ű�� file pointer�� ����(open�� �����ϸ� NULL return)
	if ((dst = fopen(argv[3], "wb")) == NULL) {//argv[3] file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		exit(1);
	}

	//src1���� 1byte ������ �ִ� MAX_BUF�� read�ؼ� buf�迭�� ����
		//������ ��� ������ 0�� return �Ͽ� while loop�� ��������	
	while ((count = fread(buf, 1, MAX_BUF, src1)) > 0) {
		//buf�κ��� 1byte�� count�� dst�� write
		fwrite(buf, 1, count, dst);
	}

	//src2���� 1byte ������ �ִ� MAX_BUF�� read�ؼ� buf�迭�� ����
		//������ ��� ������ 0�� return �Ͽ� while loop�� ��������
	while ((count = fread(buf, 1, MAX_BUF, src2)) > 0) {
		//buf�κ��� 1byte�� count�� dst�� write
		fwrite(buf, 1, count, dst);
	}

	fclose(src1);
	fclose(src2);
	fclose(dst);
}
