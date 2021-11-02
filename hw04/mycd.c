#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : change working directory program
[Description]   : argv[1] directory�� �̵�
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // �̵��� directory
[Output]        : Nothing.
[Calls]
		perror()        //���� �޼��� ���
	fprintf()	//���Ͽ� �޼��� ���
		exit()          //���α׷� ����
		chdir()         //working directory�� �Է����� ���� ���丮�� ����

*/
int
main(int argc, char* argv[])
{
	if (argc != 2) {//argc!=2�� ���α׷� ����
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	//argv[1] directory�� �̵�(���α׷� ������ ��θ� �̵��� �۾��ϴ� ���̹Ƿ� ��� ������Ʈ������ �ٲ��� ����)
	if (chdir(argv[1]) < 0) {
		//�����ϸ� �����޼��� ���
		perror("chdir");
		exit(1);
	}
}
