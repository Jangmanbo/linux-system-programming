#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

/*
[Program Name]  : directory data display program
[Description]   : ���� ���丮�� ���� �̸��� ���
[Input]		: Nothing.
[Output]        : ���� ���丮 �� ������ �̸�
[Calls]
	opendir()	//directory open, directory pointer return
		perror()        //���� �޼��� ���
		printf()       //�޼��� ���
		exit()          //���α׷� ����
		readdir()	//���丮 ������ ���� return, �����ϸ� return NULL
	closedir()	//���丮 �˻� ����
*/
int main()
{
	DIR* dp;
	struct dirent* dep;

	//���� ���丮�� pointer�� dp structure pointer�� ����
	if ((dp = opendir(".")) == NULL) {
		//�����ϸ� ���α׷� ����
		perror("opendir");
		exit(0);
	}

	//readdir�� ������ ������ directory ���� �ϳ��� file entry�� return
	//��� ������ return������ NULL�� return
	while (dep = readdir(dp)) {
		//���� �̸� ���
		printf("%s\n", dep->d_name);
	}

	closedir(dp);
}
