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
[Output]        : 데이터를 읽었다는 다른 프로세스의 메세지 출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		shmget()        //shared memory 생성
		shmat()         //physical address와 virtual address을 mapping
		shmctl()        //remove shared memory
		sprintf()       //배열에 문자열 저장
		sleep()         //n초간 대기
	semInit()	//semaphore 생성
	semWait()	//semaphore가 있으면 사용(1 감소), 없으면 기다림
	semPost()	//semaphore의 값을 1 증가
	semDestroy()	//semaphore 삭제
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

	srand(0x8888);

	//데이터를 작성하는 프로세스이므로 emptySemid를 기다림, 있으면 값을 1 감소
	if (semWait(emptySemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	//critical section이므로 mutexSemid를 기다림, 값을 1 감소
	if (semWait(mutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	//포인터 복사
	pInt = (int*)ptr;
	//데이터는 flag 다음에 있으므로 flag pointer에 4byte를 더함
	pData = ptr + sizeof(int);
	//pData에 어떤 프로세스에서 데이터를 전송하는지 정보 저장
	sprintf(pData, "This is a request from %d.", getpid());
	printf("Sent a request.....");

	//critical section 나옴, mutexSemid 1 증가
	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	//fullSemid semaphore 반납, 값을 1 증가
	if (semPost(fullSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	//emptySemid semaphore를 기다림, 있으면(다른 프로세스가 데이터를 읽었으면) 값을 1 감소
	if (semWait(emptySemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}
	//critical section이므로 mutexSemid를 기다림
	if (semWait(mutexSemid) < 0) {
		fprintf(stderr, "semWait failure\n");
		exit(1);
	}

	//어떤 프로세스가 데이터를 읽었는지 출력
	printf("Received reply: %s\n", pData);

	//critical section 나옴
	if (semPost(mutexSemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	//emptySemid semaphore 반납
	if (semPost(emptySemid) < 0) {
		fprintf(stderr, "semPost failure\n");
		exit(1);
	}

	sleep(1);

	//semaphore 삭제
	if (semDestroy(emptySemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(fullSemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}
	if (semDestroy(mutexSemid) < 0) {
		fprintf(stderr, "semDestroy failure\n");
	}

	//shared memory 삭제
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
