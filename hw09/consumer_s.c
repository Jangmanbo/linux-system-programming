#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semlib.h"
#include "prodcons.h"

/*
[Program Name]  : semaphore consumer process program
[Description]
	semaphore�� ����ȭ�� �ϴ� consumer(������ �б�) process
	��Ƽ���μ����̹Ƿ� System V�� ����
[Input]		: Nothing.
[Output]	: ��� �����͸� �а� ���μ����� ���� �� ��� �����Ͱ� �����ִ��� ���
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

	//3���� semaphore�� �ʱⰪ�� ����(empty=MAX_BUF, full=0 ,mutex=1)
	if (semInitValue(emptySemid, MAX_BUF) < 0) {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(fullSemid, 0) < 0) {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}
	if (semInitValue(mutexSemid, 1) < 0) {
		fprintf(stderr, "semInitValue failure\n");
		exit(1);
	}

	srand(0x9999);	//���� ����
	for (i = 0; i < NLOOPS; i++) {
		//consumer process�̹Ƿ� full semaphore�� ��ٸ�
		if (semWait(fullSemid) < 0) {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		//mutex semaphore�� ��ٸ�
		//�ٸ� ���μ���(producer)�� critical section�� �ִٸ� ���������⸦ ��ٸ�
		if (semWait(mutexSemid) < 0) {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data;	//dequeue
		pBuf->out = (pBuf->out + 1) % MAX_BUF;	//out pointer update
		pBuf->counter--;			//������ ���� ����

		//mutex semaphore�� ���� 1 ����
		//cirtical section�� ��������
		if (semPost(mutexSemid) < 0) {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		//empty semaphore�� ���� 1 ����
		//circular queue�� �����͸� �����ߴٴ� �ǹ�
		if (semPost(emptySemid) < 0) {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		usleep((rand() % 100) * 10000);	//���� �ð����� ���μ��� ���
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}
