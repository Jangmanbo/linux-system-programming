#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

/*
[Program Name]  : consumer process program
[Description]   : ��Ƽ���μ��� �� comsumerr(������ �б�) process
[Input]		: Nothing.
[Output]	: Nothing.
[Calls]         :
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	printf()	//�޼��� ���
	srand()		//���� ����
	shmget()	//shared memory ����
	shmat()		//physical address�� virtual address��mapping
	shmctl()	//shared memory ����
	usleep()	//�����ð����� wait(���� : ����ũ����)
*/
int
main()
{
	//��Ƽ���μ����̹Ƿ� shared memory�� ����Ű�� ������ ���
	BoundedBufferType* pBuf;
	int					shmid, i, data;

	//SHM_SIZE��ŭ�� shared memory�� �Ҵ����
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}

	//physical address�� virtual address��mapping
	if ((pBuf = (BoundedBufferType*)shmat(shmid, 0, 0)) == (void*)-1) {
		perror("shmat");
		exit(1);
	}

	srand(0x9999);
	for (i = 0; i < NLOOPS; i++) {
		//circular queue�� ���������
		if (pBuf->counter == 0) {
			printf("Consumer: Buffer empty. Waiting.....\n");
			//circular queue�� producer�κ��� �����Ͱ� ����� ������ busy waiting
			while (pBuf->counter == 0)
				;
		}

		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data;	//dequeue
		pBuf->out = (pBuf->out + 1) % MAX_BUF;	//out pointer update
		pBuf->counter--;			//������ ���� ����

		usleep((rand() % 100) * 10000);	//���� �ð����� wait
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}
