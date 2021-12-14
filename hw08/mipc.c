#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include "synclib.h"

#define	NLOOPS	10
#define	SIZE	sizeof(long)


/*
[Function Name] : update
[Description]   : 입력받은 pointer가 가리키는 값을 1 증가
[Input]
	long	*ptr	//long pointer
[Output]        : Nothing.
[Call By]	: main
[Calls]		: Nothing.
[Given]		: Nothing.
[Returns]	: 포인터가 가리키는 값에서 1 증가한 값
*/
int
update(long* ptr)
{
	return ((*ptr)++);
}

/*
[Program Name]  : my ipc program
[Description]   : shared memory로 parent, child process가 번갈아가며 1씩 증가시키는 프로그램
[Input]		: Nothing.
[Output]        : counter 변수의 값
[Calls]         :
		exit()          //프로세스 종료
		perror()        //에러 메세지 출력
		fprintf()       //파일에 메세지 출력
		mmap()          //virtual address와 file을 mapping
	open()		//open file
	close()		//close file
	fork()		//child process 생성
	update()	//값을 1 증가시키는 함수
	printf()	//메세지 출력
	TELL_WAIT()	//pipe 생성
	TELL_CHILD()	//child process에게 메세지 작성
	WAIT_CHILD()	//child process를 기다림, waiting 상태로 전환, 메세지가 오면 read
	TELL_PARENT()	//parent process에게 메세지 작성
	WAIT_PARENT()	//parent process를 기다림, waiting 상태로 전환, 메세지가 오면 read
*/
int
main()
{
	int		fd, i, counter;
	pid_t	pid;
	caddr_t	area;

	//open zero file
	if ((fd = open("/dev/zero", O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}
	//zero file을 open하고 mmap호출
	//=>physical memory에 shared memory 생성
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))
		== (caddr_t)-1) {
		perror("mmap");
		exit(1);
	}
	//close file
	close(fd);

	//pipe 생성
	TELL_WAIT();
	//child process 생성
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	else if (pid > 0) {//parent process
		for (i = 0; i < NLOOPS; i += 2) {
			//counter를 1 증가
			//올바른 counter값이 아니면 프로세스 종료
			if ((counter = update((long*)area)) != i) {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Parent: counter=%d\n", counter);
			//child process에게 메세지 작성
			TELL_CHILD();
			//child process가 메세지를 작성할 때까지 waiting 상태로 전환
			//메세지가 오면 read
			WAIT_CHILD();
		}
	}
	else {//child process
		for (i = 1; i < NLOOPS; i += 2) {
			//parent process가 메세지를 작성할 때까지 waiting 상태로 전환
						//메세지가 오면 read
			WAIT_PARENT();
			//counter를 1 증가
			//올바른 counter값이 아니면 프로세스 종료
			if ((counter = update((long*)area)) != i) {
				fprintf(stderr, "Counter mismatch\n");
				exit(1);
			}
			printf("Child : counter=%d\n", counter);
			//parent process에게 메세지 작성
			TELL_PARENT();
		}
	}
}
