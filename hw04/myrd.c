#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : remove directory program
[Description]   : argv[1] directory ����
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ������ directory
[Output]        : Nothing.
[Calls]
		perror()        //���� �޼��� ���
		fprintf()       //���Ͽ� �޼��� ���
		exit()          //���α׷� ����
		rmdir()        //�Է����� ���� ���丮 ����
*/
int main(int argc, char* argv[])
{
	if (argc != 2) {//argv!=2�� ���α׷� ����
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	//argv[1] directory ����
	if (rmdir(argv[1]) < 0) {
		//directory ���ſ� �����ϸ� ���� �޼��� ���
		perror("rmdir");
		exit(1);
	}
}
