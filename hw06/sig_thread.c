#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define	THREAD_MAIN

/*
#define	THREAD_1
#define	THREAD_2
*/



/*
[Function Name] : SigIntHandler
[Description]   :
	어떤 thread에서 signal handler를 등록해도 main thread에서signal 처리함을 보임
	프로세스 종료
[Input]         :
	signo	//수신한 signal
[Output]        : signal을 처리하는 thread의 tid 출력
[Call By]       : kernel
[Calls]         :
	printf()	//메세지 출력
	exit()		//프로세스 종료
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal by thread %ld\n", pthread_self());
	printf("Terminate this process\n");

	exit(0);
}

/*
[Function Name] : Thread1
[Description]   : SIGINT signal handler를 등록
[Input]         : dummy
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
	signal()	//signal handler를 등록
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
Thread1(void* dummy)
{
#ifdef	THREAD_1
	signal(SIGINT, SigIntHandler);
#endif

	while (1)
		;
}

/*
[Function Name] : Thread2
[Description]   : SIGINT signal handler를 등록
[Input]         : dummy
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
		signal()        //signal handler를 등록
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
Thread2(void* dummy)
{
#ifdef	THREAD_2
	signal(SIGINT, SigIntHandler);
#endif

	while (1)
		;
}

/*
[Program Name]  : main thread signal handler program
[Description]   : 어떤 thread에서 signal handler를 등록해도 main thread에서signal 처리함을 보이는 프로그램
[Input]		: Nothing.
[Output]        : main thread와 새로 생성한 thread의 tid 출력
[Calls]
	pthread_create()//thread 생성
		signal()        //signal handler를 등록
	printf()        //메세지 출력
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
	pause()		//signal이 발생할 때까지 대기
*/
int
main()
{

	pthread_t	tid1, tid2;

	//&tid1 : 생성한 thread의id
	//NULL : thread의 attribute
	//Thread1 : thread에서 실행할 함수
	//NULL : Thread1함수의 파라미터 없음
	//Thread1()를 실행하는 thread 생성
	if (pthread_create(&tid1, NULL, (void*)Thread1, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//&tid2 : 생성한 thread의id
		//NULL : thread의 attribute
		//Thread2 : thread에서 실행할 함수
		//NULL : Thread2함수의 파라미터 없음
		//Thread2()를 실행하는 thread 생성
	if (pthread_create(&tid2, NULL, (void*)Thread2, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//main thread와 새로 생성한 thread의 tid 출력
	printf("Create two threads: tid1=%ld, tid2=%ld\n", tid1, tid2);
	printf("Main thread: tid=%ld\n", pthread_self());

#ifdef	THREAD_MAIN
	signal(SIGINT, SigIntHandler);
#endif

	printf("Press ^C to quit\n");

	for (; ; )
		pause();	//signal이 발생할 때까지 대기
}
