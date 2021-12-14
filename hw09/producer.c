#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

/*
[Program Name]  : produer process program
[Description]   : 멀티프로세스 중 producer(데이터 저장) process
[Input]		: Nothing.
[Output]	: Nothing.
[Calls]         :
	exit()		//프로세스 종료
	perror()	//에러 메세지 출력
	printf()	//메세지 출력
	srand()		//난수 생성
	shmget()	//shared memory 생성
	shmat()		//physical address와 virtual address를mapping
	shmctl()	//shared memory 제어
	sleep()		//일정시간동안 wait(단위 : 초)
*/
int
main()
{
	//멀티프로세스이므로 shared memory를 가리키는 포인터 사용
	BoundedBufferType* pBuf;
	int					shmid, i, data;

	//SHM_SIZE만큼의 shared memory를 할당받음
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}
	//physical address와 virtual address를mapping
	if ((pBuf = (BoundedBufferType*)shmat(shmid, 0, 0)) == (void*)-1) {
		perror("shmat");
		exit(1);
	}


	srand(0x8888);	//난수 생성
	for (i = 0; i < NLOOPS; i++) {
		//circular queue가 차있으면
		if (pBuf->counter == MAX_BUF) {
			printf("Producer: Buffer full. Waiting.....\n");
			//circular queue에 공간이 생길 때까지 busy waiting
			while (pBuf->counter == MAX_BUF)
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;
		pBuf->buf[pBuf->in].data = data;	//enqueue
		pBuf->in = (pBuf->in + 1) % MAX_BUF;	//in pointer update
		pBuf->counter++;			//데이터 개수 증가

		usleep(data);	//process를 data만큼 sleep
	}

	printf("Producer: Produced %d items.....\n", i);

	sleep(2);	//2초간 sleep
	printf("Producer: %d items in buffer.....\n", pBuf->counter);

	//shared memory 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
