#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "prodcons.h"
#include "semlib.h"

/*
[Program Name]  : shared memory semaphore program
[Description]
	shared memory�� �ٸ� ���μ����� ���
		semaphore�� ����ȭ ����
[Input]         : Nothing.
[Output]        : �����͸� �о��ٴ� �ٸ� ���μ����� �޼��� ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		shmget()        //shared memory ����
		shmat()         //physical address�� virtual address�� mapping
		shmctl()        //remove shared memory
		sprintf()       //�迭�� ���ڿ� ����
		sleep()         //n�ʰ� ���
	semInit()	//semaphore ����
	semWait()	//semaphore�� ������ ���(1 ����), ������ ��ٸ�
	semPost()	//semaphore�� ���� 1 ����
	semDestroy()	//semaphore ����
*/
int
main()
{
	int		shmid;
	char* ptr, * pData;
	int* pInt;
	int	emptySemid, fullSemid, mutexSemid;

	//SHM_KEY������ shared memory ����, return shared memory segment id
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}

	//physical address�� virtual address�� mapping
	if ((ptr = shmat(shmid, 0, 0)) == (void*)-1) {
		perror("shmat");
		exit(1);
	}

	//semaphore ����
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

	srand(0x8888);

	//�����͸� �ۼ��ϴ� ���μ����̹Ƿ� emptySemid�� ��ٸ�, ������ ���� 1 ����
	if (semWait(emptySemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	//critical section�̹Ƿ� mutexSemid�� ��ٸ�, ���� 1 ����
	if (semWait(mutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	//������ ����
	pInt = (int*)ptr;
	//�����ʹ� flag ������ �����Ƿ� flag pointer�� 4byte�� ����
	pData = ptr + sizeof(int);
	//pData�� � ���μ������� �����͸� �����ϴ��� ���� ����
	sprintf(pData, "This is a request from %d.", getpid());
	printf("Sent a request.....");

	//critical section ����, mutexSemid 1 ����
	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	//fullSemid semaphore �ݳ�, ���� 1 ����
	if (semPost(fullSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	//emptySemid semaphore�� ��ٸ�, ������(�ٸ� ���μ����� �����͸� �о�����) ���� 1 ����
	if (semWait(emptySemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	//critical section�̹Ƿ� mutexSemid�� ��ٸ�
	if (semWait(mutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	//� ���μ����� �����͸� �о����� ���
	printf("Received reply: %s\n", pData);

	//critical section ����
	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	//emptySemid semaphore �ݳ�
	if (semPost(emptySemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	sleep(1);

	//semaphore ����
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
