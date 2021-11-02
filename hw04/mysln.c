#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : create new file(symbolic link) program
[Description]   : argv[1]�� ���� argv[2]������ symbolic link�� ����
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ���� ���ϰ� �� ���� �迭
[Output]        : hard link�� ������� �� ����
[Calls]
		perror()        //���� �޼��� ���
		fprintf()       //���Ͽ� �޼��� ���
		exit()          //���α׷� ����
		link()          //symbolic link�� �� ���� ����
*/

int main(int argc, char* argv[])
{
	if (argc != 3) {//argv!=3�̸� ���� �޼��� ��� �� ���α׷� ����
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}

	//���� ������ argv[1]�� ������ �� ���� argv[2]�� symbolic link�� ����
	if (symlink(argv[1], argv[2]) < 0) {
		//������ �����ϸ� ���� �޼��� ���
		perror("link");
		exit(1);
	}
}
