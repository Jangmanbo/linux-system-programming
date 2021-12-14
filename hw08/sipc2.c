#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h"

/*
[Program Name]  : shared memory ipc program2
[Description]   : shared memory로 다른 프로세스와 통신
[Input]         : Nothing.
[Output]        : 각 변수의 메모리 주소 출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		shmget()        //shared memory 생성
		shmat()         //physical address와 virtual address을 mapping
		shmctl()        //remove shared memory
		sprintf()       //배열에 문자열 저장
		sleep()         //n초간 대기
*/
int
main()
{
	int		shmid;
	char* ptr, * pData;
	int* pInt;

	//SHM_KEY값으로 shared memory 생성, return shared memory segment id
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}

	//physical address와 virtual address을 mapping
	if ((ptr = shmat(shmid, 0, 0)) == (void*)-1) {
		perror("shmat");
		exit(1);
	}

	//포인터 복사
	pInt = (int*)ptr;
	//데이터는 flag 다음에 있으므로 flag pointer에 4byte를 더함
	pData = ptr + sizeof(int);
	//pData에 어떤 프로세스에서 데이터를 전송하는지 정보 저장
	sprintf(pData, "This is a request from %d.", getpid());
	//데이터를 썼다는 의미로 flag에 1 저장
	*pInt = 1;
	printf("Sent a request.....");

	//다른 프로세스(sipc1)가 데이터를 읽을 때까지 대기
	while ((*pInt) == 1)
		;

	//어떤 프로세스가 데이터를 읽었는지 출력
	printf("Received reply: %s\n", pData);
}
