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
	shared memory로 다른 프로세스와 통신
	semaphore로 동기화 수행
[Input]         : Nothing.
[Output]        : 어느 프로세스로부터 데이터를 받았는지 출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		shmget()        //shared memory 생성
		shmat()         //physical address와 virtual address을 mapping
		shmctl()        //remove shared memory
	sprintf()	//배열에 문자열 저장
	sleep()		//n초간 대기
	semInit()	//semaphore 생성
	semWait()	//semaphore가 있으면 사용(1 감소), 없으면 기다림
	semPost()	//semaphore의 값을 1 증가
*/
int
main()
{
	int		shmid;
	char* ptr, * pData;
	int* pInt;
	int	emptySemid, fullSemid, mutexSemid;

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

	//semaphore 생성
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

	//semaphore 초기값 설정
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

	//포인터 복사
	pInt = (int*)ptr;
	//데이터는 flag 다음에 있으므로 flag pointer에 4byte를 더함
	pData = ptr + sizeof(int);
	//받은 데이터 출력
	printf("Received request: %s.....", pData);
	//pData에 어떤 프로세스로부터 데이터를 받았는지 정보 저장
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

	sleep(1);//1초간 대기

	//shared memory 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
