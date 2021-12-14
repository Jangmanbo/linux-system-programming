#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
[Program Name]  : memory-mapped file copy program
[Description]   : memory-mapped file�� ���� file copy
[Input]
	int argc        // argv �迭�� ũ��
		char *argv[]    // ���� ����,  �����Ͽ� ���� ������ ����
[Output]        : Nothing.
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		fprintf()       //���Ͽ� �޼��� ���
	open()		//open file
	fstat()		//���� file�� status�� �а� �������θ� return
	lseek()		//file curosr ����
	write()		//file�� ������ write
	mmap()		//virtual address�� file�� mapping
	memcpy()	//�޸��� ���� �����Ͽ� �ٿ��ֱ�
	close()		//close file
*/
int
main(int argc, char* argv[])
{
	int			fdin, fdout;
	char* src, * dst;
	struct stat	statbuf;

	if (argc != 3) {//argc!=3�̸� ���� �޼��� ��� �� ���μ��� ����
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//�����͸� �о�� ������ �б���� open
	if ((fdin = open(argv[1], O_RDONLY)) < 0) {
		perror("open");
		exit(1);
	}
	//������ ������ �ٿ����� ���� open
	//������ ���� ����, ������ ������ ��� ����
	if ((fdout = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0) {
		perror("open");
		exit(1);
	}

	//fdin ������ status�� statbuf�� ����
	if (fstat(fdin, &statbuf) < 0) {
		perror("fstat");
		exit(1);
	}

	//file cursor�� ������ ���������� �̵�
	if (lseek(fdout, statbuf.st_size - 1, SEEK_SET) < 0) {
		perror("lseek");
		exit(1);
	}
	//������ �������� ""�ۼ�
	write(fdout, "", 1);
	//=>�� size�� st_size�� 0���� ä���� ���� ���� ����

	//offset=0, size=statbubf.st_size�̹Ƿ�
	//������ ó������ ������ �о� virtual memory�� mapping
	if ((src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0))
		== (caddr_t)-1) {
		perror("mmap");
		exit(1);
	}
	//fdout������ fdin������ ũ�⸸ŭ virtual memory�� mapping
	if ((dst = mmap(0, statbuf.st_size, PROT_WRITE, MAP_SHARED, fdout, 0))
		== (caddr_t)-1) {
		perror("mmap");
		exit(1);
	}

	//�޸� ���� ����
	memcpy(dst, src, statbuf.st_size);

	//close file descriptor
	close(fdin);
	close(fdout);
}
