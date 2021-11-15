#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : number square program
[Description]   : �Է¹��� ������ ������ ���
[Input]         :
	int	argc	//�Է¹��� �迭�� ũ��
	char	*argv[]	//������ ����
[Output]        : �Է¹��� ������ ����
[Calls]
	fprintf()	//���Ͽ� �޼��� ���
	exit()		//���μ��� ����
	atoi()		//ASCII to int
	printf()	//�޼��� ���
*/
int
main(int argc, char* argv[]) {
	if (argc != 2) {//argv!=3�̸� ���α׷� ����
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//char->int ��ȯ
	int num = atoi(argv[1]);

	//���� ���
	printf("%d\n", num * num);
}
