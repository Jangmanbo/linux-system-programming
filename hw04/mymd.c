#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

/*
[Program Name]  : make directory program
[Description]   : argv[1] directory ����
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ���� ������ directory
[Output]        : ���� ���� directory
[Calls]
	perror()        //���� �޼��� ���
		fprintf()       //���Ͽ� �޼��� ���
		exit()          //���α׷� ����
	mkdir()		//���丮 ����
*/
int main(int argc, char* argv[])
{
	if (argc != 2) {//argv!=2�� ���α׷� ����
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	//argv[1] directory�� ����(permission : rwx-r-xr-x)
	if (mkdir(argv[1], 0755) < 0) {
		//directory ������ �����ϸ� ���� �޼��� ���
		perror("mkdir");
		exit(1);
	}
}
