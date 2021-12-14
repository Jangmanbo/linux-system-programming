#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W)

char	Array[ARRAY_SIZE];	//��������

/*
[Program Name]  : shared memory program
[Description]   : shared memory�� �����Ͽ� virtual address�� mapping
[Input]         : Nothing.
[Output]        : �� ������ �޸� �ּ� ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		printf()        //�޼��� ���
	malloc()	//�޸� ���� �Ҵ�
	shmget()	//shared memory ����
	shmat()		//physical address�� virtual address�� mapping
	shmctl()	//remove shared memory
*/
int
main()
{
	int		shmid;
	char* ptr, * shmptr;

	//malloc�� ���� �޸� ���� �Ҵ�
	if ((ptr = (char*)malloc(MALLOC_SIZE)) == NULL) {
		perror("malloc");
		exit(1);
	}

	//shared memory ����
	//IPC_PRIVATE : �� ���μ����� ���
	//���μ����� ����Ǹ� IPC_RMID�� ���� �ʾƵ� physical memory�� �ڵ����� �ݳ�
	//�ٸ� ���μ��������� ��� �Ұ��������� fork�� parent, child������ ��� ����
	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}
	//physical address�� virtual address�� mapping
	if ((shmptr = shmat(shmid, 0, 0)) == (void*)-1) {
		perror("shmat");
		exit(1);
	}

	//�� �������� �޸� �ּ� ���
	printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]);
	printf("Stack around %p\n", &shmid);
	printf("Malloced from %p to %p\n", ptr, ptr + MALLOC_SIZE);
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr + SHM_SIZE);

	//shared memory ����
	//���⼭�� IPC_PRIVATE���� ��������Ƿ� ���� �ʾƵ� ��
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
