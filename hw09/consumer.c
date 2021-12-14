#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "prodcons.h"

/*
[Program Name]  : consumer process program
[Description]   : 멀티프로세스 중 comsumerr(데이터 읽기) process
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
	usleep()	//일정시간동안 wait(단위 : 마이크로초)
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

	srand(0x9999);
	for (i = 0; i < NLOOPS; i++) {
		//circular queue가 비어있으면
		if (pBuf->counter == 0) {
			printf("Consumer: Buffer empty. Waiting.....\n");
			//circular queue에 producer로부터 데이터가 저장될 때까지 busy waiting
			while (pBuf->counter == 0)
				;
		}

		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data;	//dequeue
		pBuf->out = (pBuf->out + 1) % MAX_BUF;	//out pointer update
		pBuf->counter--;			//데이터 개수 감소

		usleep((rand() % 100) * 10000);	//랜덤 시간동안 wait
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}
