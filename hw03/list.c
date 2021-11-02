#include <stdio.h>
#include <stdlib.h>

#define	MAX_BUF	256

/*
[Program Name]  : line numper program
[Description]   : ������ line number�� �Բ� ���
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���α׷�, ����� ����
[Output]        : line number�� �Բ� ������ ������ ���
[Calls]         : printf, fopen, fgets, fclose
*/

int
main(int argc, char* argv[])
{
	FILE* fp;
	char	buf[MAX_BUF];
	int		line;

	if (argc != 2) {//argv��ũ�Ⱑ 2�� �ƴϸ� ���α׷� ����
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	//rt:text file�� read
	//fopen�� argv[1]�� ����Ű�� file pointer�� ����
	if ((fp = fopen(argv[1], "rt")) == NULL) {//argv[1]���� open�� ����
		perror("fopen");
		exit(1);
	}

	line = 1;
	//fgets: file pointer�� ���� ���� ���ڿ��� buf�� �� �پ� ����
	//������ ������ �о��ٸ� null pointer(=0)�� return�Ͽ� ���� ��������
	while (fgets(buf, MAX_BUF, fp)) {
		//line�� ���� ����ϰ� buf  ���(line 1 ����)
		printf("%4d: %s", line++, buf);
	}

	fclose(fp);
}
