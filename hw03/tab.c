#include <stdio.h>
#include <string.h>

enum { FALSE, TRUE };

/*
[Function Name] : convert tab to 4 spaces
[Description]	: tab�� 4���� �������� �����Ͽ� ����
[Input]
		char	*fname	//tab�� 4���� �������� �ٲ� ����
[Output]	: Nothing.
[Call By]	: main()
[Calls]         : fopen, fgetc, fputc, fclose
[Given]		: Nothing
[Returns]	: Nothing
*/
void
conv(char* fname)
{
	FILE* src, * dst;
	char	fdest[40];
	int		ch, first;

	//rt:text file�� read
	//fopen : text file�� open, ������ ����Ű�� �����͸� return
	if ((src = fopen(fname, "rt")) == NULL) {//file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		return;
	}

	strcpy(fdest, fname);	//fdest�� fname ����
	strcat(fdest, ".t");	//fdest�� .t�� �̾����

	//wt:text file��write
		//fopen : text file�� open, ������ ����Ű�� �����͸� return
	if ((dst = fopen(fdest, "wt")) == NULL) {//file open�� �����ϸ� ���α׷� ����
		perror("fopen");
		return;
	}

	first = TRUE;
	//fgetc : src file pointer�� �о�� char�� return, ������ ������ ������ EOF�� return
	//������ ������ ������ ���� ��������
	while ((ch = fgetc(src)) != EOF) {
		//���� char�� tabŰ�� ���� 4���� dst file pointer�� ���� ���Ͽ� write
		if (first && ch == '\t') {
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		}
		else { //char�� tabŰ�� �ƴϸ� �״�� write
			fputc(ch, dst);
			//line���� tabŰ�� �ƴ� ���ڰ� ó������ �������� first�� FALSE�� �ٲپ� �ش� line���� ���̻� tabŰ�� 4���� �������� �ٲ��� ����
			if (first)
				first = FALSE;
			//���� line���� �Ѿ�� �ٽ� tabŰ�� 4���� �������� �ٲ� �� �ְ� ��
			if (ch == '\n')
				first = TRUE;
		}
	}

	fclose(src);
	fclose(dst);
}

/*
[Program Name]  : tab to spcae
[Description]   : tab�� ���� 4���� �ٲٴ� ���α׷�
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, tab�� �������� �ٲ� ����
[Output]        : Nothing.
[Calls]         : Nothing.
*/

int
main(int argc, char* argv[])
{
	while (--argc) {
		conv(argv[argc]);
	}
}
