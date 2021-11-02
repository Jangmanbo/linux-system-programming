#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define	MAX_BUF	256

/*
[Program Name]  : print working directory program
[Description]   : ���� ���丮�� ��� ���
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]
[Output]        : Nothing.
[Calls]
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		exit()          //���α׷� ����
		getcwd()        //���� �۾� ���丮�� full path�� buf�� ����

*/
int
main(int argc, char* argv[])
{
	char	buf[MAX_BUF];

	//���� �۾� ���丮�� full path�� buf�� ����
	if (getcwd(buf, MAX_BUF) == NULL) {
		//�����ϸ� ���� �޼��� ���
		perror("getcwd");
		exit(1);
	}

	//���� �۾� ���丮�� full path ���
	printf("%s\n", buf);
}
