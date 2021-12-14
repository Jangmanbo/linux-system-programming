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
[Output]        : ��� ���μ����κ��� �����͸� �޾Ҵ��� ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		shmget()        //shared memory ����
		shmat()         //physical address�� virtual address�� mapping
		shmctl()        //remove shared memory
	sprintf()	//�迭�� ���ڿ� ����
	sleep()		//n�ʰ� ���
	semInit()	//semaphore ����
	semWait()	//semaphore�� ������ ���(1 ����), ������ ��ٸ�
	semPost()	//semaphore�� ���� 1 ����
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

	//semaphore �ʱⰪ ����
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

	srand(0x9999);

	if (semWait(fullSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	if (semWait(mutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	//������ ����
	pInt = (int*)ptr;
	//�����ʹ� flag ������ �����Ƿ� flag pointer�� 4byte�� ����
	pData = ptr + sizeof(int);
	//���� ������ ���
	printf("Received request: %s.....", pData);
	//pData�� � ���μ����κ��� �����͸� �޾Ҵ��� ���� ����
	sprintf(pData, "This is a reply from %d.", getpid());
	printf("Replied.\n");

	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}
	if (semPost(emptySemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	sleep(1);//1�ʰ� ���

	//shared memory ����
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
