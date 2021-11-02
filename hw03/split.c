#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define	MAX_BUF	1024

/*
[Function Name] : file size
[Description]	: file�� size�� ���
[Input]
		FILE *fp	//size�� ����� ������ ������
[Output]
	long size	//file�� ũ��
[Call By]	: main()
[Calls]         : ftell, fseek, ftell
[Given]		: Nothing
[Returns]	: Nothing
*/
long
filesize(FILE* fp)
{
	long	cur, size;

	//���� file pointer�� ��ġ�� ����
	//�� ���α׷����� filesize�Լ��� ������ open�� ���� ���
	//�� file pointer�� ���� ������ ó���� ����	
	cur = ftell(fp);

	//file pointer�� ������ ������ ����
	fseek(fp, 0L, SEEK_END);

	//file pointer�� ���� ��ġ�� return, �� ������ ũ��
	size = ftell(fp);

	//file pointer�� ���� ��ġ�� ��������
	fseek(fp, cur, SEEK_SET);

	//file�� size�� return
	return(size);
}

/*
[Program Name]  : binary file split program
[Description]   : argv[1] ������ argv[2], argv[3] ���Ϸ� ����
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, split�� ����, split�Ͽ� ���� ������ ����-1, ����-2
[Output]        : split�Ͽ� ���� ������ 2���� binary file
[Calls]         : fseek, fprintf, fopen, fread, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* src, * dst1, * dst2;
	char	buf[MAX_BUF];
	int		count, size;

	if (argc != 4) {//argv!=4�� ���α׷� ����
		fprintf(stderr, "Usage: %s source dest1 dest2\n", argv[0]);
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
	if ((dst1 = fopen(argv[2], "wb")) == NULL) {//argv[2] file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		exit(1);
	}

	//wb : binary file�� write
		//fopen : argv[3]�� ����Ű�� file pointer�� ����(open�� �����ϸ� NULL return)
	if ((dst2 = fopen(argv[3], "wb")) == NULL) {//argv[3] file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		exit(1);
	}

	//�������� file�� size��2�� ������ size variable�� ����
	size = filesize(src) / 2;

	//src�κ��� �ִ� MAX_BUF��ŭ �а� dst1�� write
	//������ ���� ��ŭ size ���� �����ϴٰ� 0�� �Ǹ�, �� ������ �� ������ ������ ��������
	while (size > 0) {
		count = (size > MAX_BUF) ? MAX_BUF : size;
		fread(buf, 1, count, src);
		fwrite(buf, 1, count, dst1);
		size -= count;
	}

	//src�κ��� �ִ� MAX_BUF��ŭ �а� dst2�� write
	//������ ������ �� ������ fread�� 0�� �����Ͽ� ������ ��������
	while ((count = fread(buf, 1, MAX_BUF, src)) > 0) {
		fwrite(buf, 1, count, dst2);
	}

	fclose(src);
	fclose(dst1);
	fclose(dst2);
}
