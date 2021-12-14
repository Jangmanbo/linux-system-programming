#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

#define	ARRAY_SIZE	40000
#define	MALLOC_SIZE	100000
#define	SHM_SIZE	100000
#define	SHM_MODE	(SHM_R | SHM_W)

char	Array[ARRAY_SIZE];	//전역변수

/*
[Program Name]  : shared memory program
[Description]   : shared memory를 생성하여 virtual address와 mapping
[Input]         : Nothing.
[Output]        : 각 변수의 메모리 주소 출력
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
	malloc()	//메모리 동적 할당
	shmget()	//shared memory 생성
	shmat()		//physical address와 virtual address을 mapping
	shmctl()	//remove shared memory
*/
int
main()
{
	int		shmid;
	char* ptr, * shmptr;

	//malloc을 통해 메모리 동적 할당
	if ((ptr = (char*)malloc(MALLOC_SIZE)) == NULL) {
		perror("malloc");
		exit(1);
	}

	//shared memory 생성
	//IPC_PRIVATE : 이 프로세스만 사용
	//프로세스가 종료되면 IPC_RMID를 하지 않아도 physical memory도 자동으로 반납
	//다른 프로세스끼리는 사용 불가능하지만 fork로 parent, child끼리는 사용 가능
	if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0) {
		perror("shmget");
		exit(1);
	}
	//physical address와 virtual address을 mapping
	if ((shmptr = shmat(shmid, 0, 0)) == (void*)-1) {
		perror("shmat");
		exit(1);
	}

	//각 변수마다 메모리 주소 출력
	printf("Array[] from %p to %p\n", &Array[0], &Array[ARRAY_SIZE]);
	printf("Stack around %p\n", &shmid);
	printf("Malloced from %p to %p\n", ptr, ptr + MALLOC_SIZE);
	printf("Shared memory attached from %p to %p\n", shmptr, shmptr + SHM_SIZE);

	//shared memory 삭제
	//여기서는 IPC_PRIVATE으로 만들었으므로 하지 않아도 됨
	if (shmctl(shmid, IPC_RMID, 0) < 0) {
		perror("shmctl");
		exit(1);
	}
}
