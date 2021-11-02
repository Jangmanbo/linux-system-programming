#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

/*
[Program Name]  : file mode change program
[Description]   : file mode�� ����
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
	stat()		//file status�� buffer�� ����, �������θ� return
	chmod()		//file mode ����, �������θ� return
*/
int main()
{
	struct stat	statbuf;	//file status�� ������ buffer

	//bar file�� status�� statbuf�� ����
	if (stat("bar", &statbuf) < 0) {
		//�����ϸ� ���� �޼��� ���
		perror("stat");
		exit(1);
	}

	//bar file�� mode�� ����
	//& ~S_IWGRP : ���� file mode���� S_IWGRP ��Ʈ�� 0���� ����
	//| S_ISUID : ���� file mode���� S_ISUID ��Ʈ�� 1�� ����
	if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0) {
		//�����ϸ� ���� �޼��� ���
		perror("chmod");
		exit(1);
	}

	//foo file�� mode�� S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH ��Ʈ�� 1, ������ ��Ʈ�� 0���� ����
	if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
		//�����ϸ� ���� �޼��� ���
		perror("chmod");
		exit(1);
	}
}
