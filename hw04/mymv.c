#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : file move program
[Description]   : argv[1]�� argv[2]�� �̸� ������ ���� ���丮 �̵�
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // �̸��� �ٲ� ���ϰ� ���� �ٲ� �̸��� ����
[Output]        : Nothing.
[Calls]
		perror()        //���� �޼��� ���
		fprintf()       //���Ͽ� �޼��� ���
		exit()          //���α׷� ����
		rename()        //���� �̸� ����

*/
int main(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}

	//argv[1]�� argv[2]�� �̸� ����
	//���ϳ���, ���丮���� ���� ����(����-> ���丮, ���丮 -> ���� �Ұ���)
	if (rename(argv[1], argv[2]) < 0) {
		//�����ϸ� ���� �޼��� ���
		perror("rename");
		exit(1);
	}
}
