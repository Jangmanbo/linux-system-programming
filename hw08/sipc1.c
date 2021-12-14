#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h"


/*
[Program Name]  : shared memory ipc program1
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
	sprintf()	//�迭�� ���ڿ� ����
	sleep()		//n�ʰ� ���
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
	//�ٸ� ���μ���(sipc2)�� �����͸� �ۼ��Ͽ� pInt(flag)�� 0���� �ٲ� ������ wait
	while ((*pInt) == 0)
		;

	//�����ʹ� flag ������ �����Ƿ� flag pointer�� 4byte�� ����
	pData = ptr + sizeof(int);
	//���� ������ ���
	printf("Received request: %s.....", pData);
	//pData�� � ���μ����κ��� �����͸� �޾Ҵ��� ���� ����
	sprintf(pData, "This is a reply from %d.", getpid());
	//�����͸� �о��ٴ� �ǹ̿��� flag�� 0���� ����
	*pInt = 0;
	printf("Replied.\n");

	sleep(1);//1�ʰ� ���

	//shared memory ����
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
