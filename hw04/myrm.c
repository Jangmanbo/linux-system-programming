#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : file remove program
[Description]   : argv[1]~argv[argc-1] ���ϵ� ����
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ ���ϵ��� �迭
[Output]        : Nothing.
[Calls]
		perror()        //���� �޼��� ���
		fprintf()       //���Ͽ� �޼��� ���
		exit()          //���α׷� ����
		remove()        //�Է����� ���� ���� ����
*/
int main(int argc, char* argv[])
{
	int		i;

	if (argc == 1) {//������ ������ ���ٸ� ���α׷� ����
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}

	for (i = 1; i < argc; i++) {
		if (remove(argv[i]) < 0) {//���� ����
			//�����ϸ� ���� �޼��� ���
			perror("remove");
			exit(1);
		}
	}
}
