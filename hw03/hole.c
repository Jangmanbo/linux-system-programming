#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : file create program
[Description]   : file.hole�̶�� 1024byte ���� ���� ����� ���α׷�
[Input]		: Nothing.
[Output]        : ���� ���� 1024byte ���� ����(file.hole)
[Calls]         : creat, lseek, write, close
*/
int
main()
{
	int 	fd;
	char	ch = '\0';

	//file.hole�̶�� ����ִ� ���� ����
	//0400:������ �б� ����
	if ((fd = creat("file.hole", 0400)) < 0) {
		perror("creat");
		exit(1);
	}

	//file pointer�� ������ ���� �տ������� 1023byte �̵�
	if (lseek(fd, 1023, SEEK_SET) < 0) {
		perror("lseek");
		exit(1);
	}
	write(fd, &ch, 1); //������ 1byte�� '\0'�� write

	close(fd);
}
