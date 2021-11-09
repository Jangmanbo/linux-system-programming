#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char* argv[])
{
	FILE* src;
	char ch;
	int count = 0;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//argv[1]������ text read ���� open
	if ((src = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	}

	printf("%c\n", *argv[2]);

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
