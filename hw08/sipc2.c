#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h"

/*
[Program Name]  : shared memory ipc program2
[Description]   : shared memory�� �ٸ� ���μ����� ���
[Input]         : Nothing.
[Output]        : �� ������ �޸� �ּ� ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
		shmget()        //shared memory ����
		shmat()         //physical address�� virtual address�� mapping
		shmctl()        //remove shared memory
		sprintf()       //�迭�� ���ڿ� ����
		sleep()         //n�ʰ� ���
*/
int
main()
{
	int		shmid;
	char* ptr, * pData;
	int* pInt;

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

	//������ ����
	pInt = (int*)ptr;
	//�����ʹ� flag ������ �����Ƿ� flag pointer�� 4byte�� ����
	pData = ptr + sizeof(int);
	//pData�� � ���μ������� �����͸� �����ϴ��� ���� ����
	sprintf(pData, "This is a request from %d.", getpid());
	//�����͸� ��ٴ� �ǹ̷� flag�� 1 ����
	*pInt = 1;
	printf("Sent a request.....");

	//�ٸ� ���μ���(sipc1)�� �����͸� ���� ������ ���
	while ((*pInt) == 1)
		;

	//� ���μ����� �����͸� �о����� ���
	printf("Received reply: %s\n", pData);
}
