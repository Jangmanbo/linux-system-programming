#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
[Program Name]  : file mode creation mask program
[Description]   : file mode�� Ư�� ��Ʈ�� ���� bar, foo file�� ����
[Input]		: Nothing.
[Output]        : Nothing
[Calls]
		perror()        //���� �޼��� ���
	exit()		//���α׷� ����
	umask()		//file mode���� �Է����� ���� ��Ʈ�� ���� �����ϵ��� mask
	creat()		//�Է����� ���� �̸��� permission�������� ����, �������θ� return
*/
int main()
{
	umask(0);	//mask�� �ϳ��� ���� ����
	//file mode�� rw-rw-rw�� bar file ����
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
		S_IROTH | S_IWOTH) < 0) {
		//file ������ �����ϸ� ���� �޼��� ���
		perror("creat");
		exit(1);
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);	//file mode���� ---rw-rw-�� �ش��ϴ� ��Ʈ�� ���� �����ϵ��� mask
	//file mode�� rw-rw-rw-�� �Ϸ� ������ ---rw-rw- ��Ʈ�� ���� �ϹǷ� rw-------�� foo file ����
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
		S_IROTH | S_IWOTH) < 0) {
		//file ������ �����ϸ� ���� �޼��� ���
		perror("creat");
		exit(1);
	}
}
