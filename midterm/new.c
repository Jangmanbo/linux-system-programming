#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#define	MAX_PATH	256

/*
[Function Name] : JustDoIt
[Description]   : ����� �ð��� ���� �ֱ��� ���� �̸� ���
[Input]
		char	*path	 : �����͸� ���� ���丮�� ���
[Output]	: ����� �ð��� ���� �ֱ��� ���� �̸�
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

	long int modify = 0;	//���� ���� �ð�
	char fname[100];	//���� �̸�

	//readdir�� ������ ������ directory ���� �ϳ��� �����͸� return
	//��� �����͸� return������ NULL�� return
	while (dep = readdir(dp)) {
		//., ..�� ��� X
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		//statbuf���� ���Ͽ� ���� status�� �о��
		if (stat(dep->d_name, &statbuf) < 0) {
			perror("lstat");
			exit(1);
		}
		//���ݱ����� ���� ���� �ð� ���� �ֱ��̸� ����
		if (statbuf.st_mtime > modify) {
			strcpy(fname, dep->d_name);
			modify = statbuf.st_atime;
		}
	}

	//���� ���� �ð��� ���� �ֱ��� ���� �̸� ���
	printf("%s\n", fname);

	closedir(dp);
}

/*
[Program Name]  : last modify file name print program
[Description]   : ����� �ð��� ���� �ֱ��� ���� �̸� ���
[Input]         :
	int	argc	//�Է��� �迭�� ũ��
	char	*argv	//���α׷�
[Output]        : ����� �ð��� ���� �ֱ��� ���� �̸�
[Calls]
	JustDoIt()	//����� �ð��� ���� �ֱ��� ���� �̸��� ���
*/
int
main(int argc, char* argv[])
{
	if (argc != 1) {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}
	JustDoIt(".");	//���� ���丮�� �����͸� ���
}
