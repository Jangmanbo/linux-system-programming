#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : char count program
[Description]   : argv[1] ���Ͽ�argv[2]�� �� �� �ִ��� ���� ���α׷�
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ���ϰ� ����
[Output]        : argv[1] ���Ͽ�argv[2]�� �� �� �ִ���
[Calls]         :
	fprintf()	//���Ͽ� �޼��� ���
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	printf()	//�޼��� ���
	fgetc()		//���Ͽ��� �� ���ھ� �о��
	fopen()		//file open
	fclose()	//file close
*/

int
main(int argc, char* argv[])
{
	FILE* src;
	char ch;
	int count = 0;

	if (argc != 3) {//argv!=3�̸� ���α׷� ����
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//argv[1]������ text read ���� open
	if ((src = fopen(argv[1], "rt")) == NULL) {
		//�����ϸ� ���μ��� ����
		perror("fopen");
		exit(1);
	}


	//argv[1]������ �ѱ��ھ� �о��, �� ������ loop ��������
	while ((ch = fgetc(src)) != EOF) {
		//�о�� char�� argv[2]�� ��ġ�ϸ� count 1 ����
		if (ch == *argv[2]) {
			count++;
		}
	}

	//argv[1]���� ���� argv[2]�� ���� ���
	printf("%d\n", count);
	fclose(src);
}
