#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include "synclib.h"

#define	NLOOPS	10
#define	SIZE	sizeof(long)


/*
[Function Name] : update
[Description]   : �Է¹��� pointer�� ����Ű�� ���� 1 ����
[Input]
	long	*ptr	//long pointer
[Output]        : Nothing.
[Call By]	: main
[Calls]		: Nothing.
[Given]		: Nothing.
[Returns]	: �����Ͱ� ����Ű�� ������ 1 ������ ��
*/
int
update(long* ptr)
{
	return ((*ptr)++);
}

/*
[Program Name]  : my ipc program
[Description]   : shared memory�� parent, child process�� �����ư��� 1�� ������Ű�� ���α׷�
[Input]		: Nothing.
[Output]        : counter ������ ��
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		fprintf()       //���Ͽ� �޼��� ���
		mmap()          //virtual address�� file�� mapping
	open()		//open file
	close()		//close file
	fork()		//child process ����
	update()	//���� 1 ������Ű�� �Լ�
	printf()	//�޼��� ���
	TELL_WAIT()	//pipe ����
	TELL_CHILD()	//child process���� �޼��� �ۼ�
	WAIT_CHILD()	//child process�� ��ٸ�, waiting ���·� ��ȯ, �޼����� ���� read
	TELL_PARENT()	//parent process���� �޼��� �ۼ�
	WAIT_PARENT()	//parent process�� ��ٸ�, waiting ���·� ��ȯ, �޼����� ���� read
*/
int
main()
{
	int		fd, i, counter;
	pid_t	pid;
	caddr_t	area;

	//open zero file
	if ((fd = open("/dev/zero", O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}
	//zero file�� open�ϰ� mmapȣ��
	//=>physical memory�� shared memory ����
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
		== (caddr_t)-1) {
		perror("mmap");
		exit(1);
	}
	//close file
	close(fd);

	//pipe ����
	TELL_WAIT();
	//child process ����
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if (pid > 0) {//parent process
		for (i = 0; i < NLOOPS; i += 2) {
			//counter�� 1 ����
			//�ùٸ� counter���� �ƴϸ� ���μ��� ����
			if ((counter = update((long*)area)) != i) {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			//child process���� �޼��� �ۼ�
			TELL_CHILD();
			//child process�� �޼����� �ۼ��� ������ waiting ���·� ��ȯ
			//�޼����� ���� read
			WAIT_CHILD();
		}
	}
	else {//child process
		for (i = 1; i < NLOOPS; i += 2) {
			//parent process�� �޼����� �ۼ��� ������ waiting ���·� ��ȯ
						//�޼����� ���� read
			WAIT_PARENT();
			//counter�� 1 ����
			//�ùٸ� counter���� �ƴϸ� ���μ��� ����
			if ((counter = update((long*)area)) != i) {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			//parent process���� �޼��� �ۼ�
			TELL_PARENT();
		}
	}
}
