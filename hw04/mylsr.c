#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#define	MAX_PATH	256

/*
[Function Name] : JustDoIt
[Description]   : ���丮�� ������ ���
[Input]
		char	*path	 : �����͸� ���� ���丮�� ���
[Output]	: ���丮�� ������
[Call By]       : main()
[Calls]
		opendir()       //directory open, directory pointer return
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		exit()          //���α׷� ����
		readdir()       //���丮 ������ ���� return, �����ϸ� return NULL
	rewinddir()	//���丮 ���� �б� ��ġ�� ó������ �̵�
	lstat()		//file status�� �ϰ� �������θ� return
	closedir()      //���丮 �˻� ����
	strcmp()	//�� ���ڿ��� ũ�� ��
	strcpy()	//���ڿ� ����
	strcat()	//���ڿ� �̾���̱�
	JustDoIt()	//��� ȣ��

[Given]         : Nothing
[Returns]       : Nothing
*/
void
JustDoIt(char* path)
{
	DIR* dp;
	struct dirent* dep;
	struct stat		statbuf;
	char			fullpath[MAX_PATH];

	//���� ���丮�� pointer�� dp structure pointer�� ����	
	if ((dp = opendir(path)) == NULL) {
		//�����ϸ� ����
		perror("opendir");
		exit(0);
	}

	printf("\n%s:\n", path);

	//readdir�� ������ ������ directory ���� �ϳ��� �����͸� return
		//��� �����͸� return������ NULL�� return
	while (dep = readdir(dp)) {
		//., ..�� ��� X
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		//���� �̸� ���
		printf("%s\n", dep->d_name);
	}

	//���丮 ���� �б� ��ġ�� ó������ �̵�
	rewinddir(dp);

	//readdir�� ������ ������ directory ���� �ϳ��� �����͸� return
		//��� �����͸� return������ NULL�� return
	while (dep = readdir(dp)) {
		//., ..�� ��� X
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, path);	//fullpath�� path ���ڿ� ����
		strcat(fullpath, "/");	//fullpath�� /�� �̾����
		strcat(fullpath, dep->d_name);	//fullpath�� ���� �̸��� �̾����
		//statbuf�� fullpath�� ���� status�� �о��
		if (lstat(fullpath, &statbuf) < 0) {
			perror("lstat");
			exit(1);
		}

		//directory�� ��� ȣ��
		if (S_ISDIR(statbuf.st_mode)) {
			JustDoIt(fullpath);
		}
	}

	closedir(dp);
}

/*
[Program Name]  : directory data display program
[Description]   : ���� ���丮�� ����, ���丮 �̸��� ���
[Input]         : Nothing.
[Output]        : ���� ���丮 �� ����, ���丮�� �̸�
[Calls]
	JustDoIt()	//���� ���丮�� �����͸� ���
*/
int
main()
{
	JustDoIt(".");	//���� ���丮�� �����͸� ���
}
