#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include "shm.h"


/*
[Program Name]  : shared memory ipc program1
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
	sprintf()	//배열에 문자열 저장
	sleep()		//n초간 대기
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
	//다른 프로세스(sipc2)가 데이터를 작성하여 pInt(flag)를 0으로 바꿀 때까지 wait
	while ((*pInt) == 0)
		;

	//데이터는 flag 다음에 있으므로 flag pointer에 4byte를 더함
	pData = ptr + sizeof(int);
	//받은 데이터 출력
	printf("Received request: %s.....", pData);
	//pData에 어떤 프로세스로부터 데이터를 받았는지 정보 저장
	sprintf(pData, "This is a reply from %d.", getpid());
	//데이터를 읽었다는 의미에서 flag를 0으로 저장
	*pInt = 0;
	printf("Replied.\n");

	sleep(1);//1초간 대기

	//shared memory 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
