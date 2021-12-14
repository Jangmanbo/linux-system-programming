#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

/*
[Program Name]  : semaphore producer process program
[Description]
	semaphore�� ����ȭ�� �ϴ� producer(������ ����) process
	��Ƽ���μ����̹Ƿ� System V�� ����
[Input]		: Nothing.
[Output]	: ��� �����͸� �����ߴ���,  ���μ����� ���� �� ��� �����Ͱ� �����ִ��� ���
[Calls]         :
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	printf()	//�޼��� ���
	srand()		//���� ����
	shmget()	//shared memory ����
	shmat()		//physical address�� virtual address��mapping
	shmctl()	//shared memory ����
	semInit()	//semaphore ����
	semWait()	//semaphore�� ������ ���(1 ����), ������ ��ٸ�
	semPost()	//semaphore�� ���� 1 ����
	semDestroy()	//semaphore ����
	sleep()		//�����ð����� wait(���� : ��)
	usleep()        //�����ð����� wait(���� : ����ũ����)
*/
int
main()
{
	BoundedBufferType* pBuf;
	int					shmid, i, data;
	int					emptySemid, fullSemid, mutexSemid;

	//shared memory ����
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}

	//physical address�� virtual address��mapping
	if ((pBuf = (BoundedBufferType*)shmat(shmid, 0, 0)) == (void*)-1) {
		perror("shmat");
		exit(1);
	}

	//empty, full counting semaphore 2���� mutex binary semaphore 1���� ����
	//critical section�� ��ȣ
	if ((emptySemid = semInit(EMPTY_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((fullSemid = semInit(FULL_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}
	if ((mutexSemid = semInit(MUTEX_SEM_KEY)) < 0) {
		fprintf(stderr, "semInit failure\n");
		exit(1);
	}

	srand(0x8888);	//���� ����
	for (i = 0; i < NLOOPS; i++) {
		//producer process�̹Ƿ� empty semaphore�� ��ٸ�
		if (semWait(emptySemid) < 0) {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		//mutex semaphore�� ��ٸ�
		//�ٸ� ���μ���(consumer)�� critical section�� �ִٸ� ���������⸦ ��ٸ�
		if (semWait(mutexSemid) < 0) {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;		//���� ������ ����
		pBuf->buf[pBuf->in].data = data;	//enqueue
		pBuf->in = (pBuf->in + 1) % MAX_BUF;	//in counter update
		pBuf->counter++;			//������ ���� ����

		//mutex semaphore�� ���� 1 ����
		//cirtical section�� ��������
		if (semPost(mutexSemid) < 0) {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		//full semaphore�� ���� 1 ����
		//circular queue�� �����͸� �����ߴٴ� �ǹ�
		if (semPost(fullSemid) < 0) {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}
		usleep(data);	//���μ����� ���� �ð����� wait
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2);	//���μ����� 2�ʰ� ���
	printf("Producer: %d items in buffer.....\n", pBuf->counter);

	//�����ߴ� 3���� semaphore ����
	if (semDestroy(emptySemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(fullSemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(mutexSemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}
	//shared memory ����
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
