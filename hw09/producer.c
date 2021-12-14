#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

/*
[Program Name]  : produer process program
[Description]   : ��Ƽ���μ��� �� producer(������ ����) process
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
	sleep()		//�����ð����� wait(���� : ��)
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


	srand(0x8888);	//���� ����
	for (i = 0; i < NLOOPS; i++) {
		//circular queue�� ��������
		if (pBuf->counter == MAX_BUF) {
			printf("Producer: Buffer full. Waiting.....\n");
			//circular queue�� ������ ���� ������ busy waiting
			while (pBuf->counter == MAX_BUF)
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;
		pBuf->buf[pBuf->in].data = data;	//enqueue
		pBuf->in = (pBuf->in + 1) % MAX_BUF;	//in pointer update
		pBuf->counter++;			//������ ���� ����

		usleep(data);	//process�� data��ŭ sleep
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2);	//2�ʰ� sleep
	printf("Producer: %d items in buffer.....\n", pBuf->counter);

	//shared memory ����
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
