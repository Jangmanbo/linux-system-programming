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
	semaphore로 동기화를 하는 consumer(데이터 읽기) process
	멀티프로세스이므로 System V로 구현
[Input]		: Nothing.
[Output]	: 몇개의 데이터를 읽고 프로세스가 끝날 때 몇개의 데이터가 남아있는지 출력
[Calls]         :
	exit()		//프로세스 종료
	perror()	//에러 메세지 출력
	printf()	//메세지 출력
	srand()		//난수 생성
	shmget()	//shared memory 생성
	shmat()		//physical address와 virtual address를mapping
	shmctl()	//shared memory 제어
	semInit()	//semaphore 생성
	semWait()	//semaphore가 있으면 사용(1 감소), 없으면 기다림
	semPost()	//semaphore의 값을 1 증가
	semDestroy()	//semaphore 삭제
	sleep()		//일정시간동안 wait(단위 : 초)
	usleep()        //일정시간동안 wait(단위 : 마이크로초)
*/
int
main()
{
	BoundedBufferType* pBuf;
	int					shmid, i, data;
	int					emptySemid, fullSemid, mutexSemid;

	//shared memory 생성
	if ((shmid = shmget(SHM_KEY, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}

	//physical address와 virtual address를mapping
	if ((pBuf = (BoundedBufferType*)shmat(shmid, 0, 0)) == (void*)-1) {
		perror("shmat");
		exit(1);
	}

	//empty, full counting semaphore 2개와 mutex binary semaphore 1개를 생성
	//critical section을 보호
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

	//3개의 semaphore의 초기값을 설정(empty=MAX_BUF, full=0 ,mutex=1)
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

	srand(0x9999);	//난수 생성
	for (i = 0; i < NLOOPS; i++) {
		//consumer process이므로 full semaphore를 기다림
		if (semWait(fullSemid) < 0) {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		//mutex semaphore를 기다림
		//다른 프로세스(producer)가 critical section에 있다면 빠져나오기를 기다림
		if (semWait(mutexSemid) < 0) {
			fprintf(stderr, "semWait failure\n");
			exit(1);
		}
		printf("Consumer: Consuming an item.....\n");
		data = pBuf->buf[pBuf->out].data;	//dequeue
		pBuf->out = (pBuf->out + 1) % MAX_BUF;	//out pointer update
		pBuf->counter--;			//데이터 개수 감소

		//mutex semaphore의 값을 1 증가
		//cirtical section을 빠져나옴
		if (semPost(mutexSemid) < 0) {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		//empty semaphore의 값을 1 증가
		//circular queue에 데이터를 저장했다는 의미
		if (semPost(emptySemid) < 0) {
			fprintf(stderr, "semPost failure\n");
			exit(1);
		}

		usleep((rand() % 100) * 10000);	//랜덤 시간동안 프로세스 대기
	}

	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", pBuf->counter);
}
