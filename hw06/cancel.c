#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


pthread_t	ThreadId[2];

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
	perror()	//에러 메세지 출력
	exit()		//프로세스 종료
	pthread_cancel()//thread 중단
	pthread_join()	//thread의 종료를 기다림
[Given]         : ThreadId[2]
[Return]        : Nothing.
*/
void
SigIntHandler(int signo)
{
	int		i;

	//현재 스레드 ID와 새로 생성한 두 thread terminate 메세지 출력
	printf("Received a SIGINT signal by thread %ld\n", pthread_self());
	printf("Terminate other threads: tid=%ld, %ld\n", ThreadId[0], ThreadId[1]);

	//두 thread 중단
	for (i = 0; i < 2; i++) {
		if (pthread_cancel(ThreadId[i])) {
			perror("pthread_cancel");
			exit(1);
		}
	}
	//각 thread의 종료를 기다림
	for (i = 0; i < 2; i++) {
		if (pthread_join(ThreadId[i], NULL)) {
			perror("pthread_join");
			exit(1);
		}
	}
	//두 thread가 종료되었음을 알림
	printf("Threads terminated: tid=%ld, %ld\n", ThreadId[0], ThreadId[1]);

	exit(0);	//프로세스 종료
}

/*
[Function Name] : Thread1
[Description]   : thread1의 취소 요청 시 동작을 설정
[Input]         : dummy
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
	pthread_setcancelstate()	//pthread_cancel에 의해 취소 요청을 받았을 때 어떻게 반응할지 설정
	pthread_setcanceltype()		//취소 요청을 받아들일 때 어느 지점에서 종료할지 설정
	pthread_exit()			//thread 종료
	perror()			//에러 메세지 출력
	printf()			//메세지 출력
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
Thread1(void* dummy)
{
	printf("Thread %ld created.....\n", pthread_self());

	//취소 요청을 받아들이기로 설정
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}

	//thread 즉시 종료하기로 설정
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	//time quantum이 다 할 때까지 실행
	while (1)
		;
}

/*
[Function Name] : Thread2
[Description]   : thread2의 취소 요청 시 동작을 설정
[Input]         : dummy
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
		pthread_setcancelstate()        //pthread_cancel에 의해 취소 요청을 받았을 때 어떻게 반응할지 설정
		pthread_setcanceltype()         //취소 요청을 받아들일 때 어느 지점에서 종료할지 설정
		pthread_exit()                  //thread 종료
		perror()                        //에러 메세지 출력
		printf()                        //메세지 출력
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
Thread2(void* dummy)
{
	printf("Thread %ld created.....\n", pthread_self());

	//취소 요청을 받아들이기로 설정
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	//thread를 즉시 종료하기로 설정
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	//time quantum이 다 할 때까지 실행
	while (1)
		;
}

/*
[Program Name]  : thread terminate program
[Description]   :
	두 개의 thread 생성하고 취소 요청 시 동작 설정
	thread1, thread2가 번갈아서 time quantum이 다 할 때까지 돌다가
	Ctrl+C로 SIGINT signal을 전달하면 스레드 종료
[Input]         : Nothing.
[Output]        : 두 개의 thread 생성
[Calls]
	pthread_create()//thread 생성
	perror()	//에러 메세지 출력
	exit()		//프로그램 종료
	signal()	//signal handler 등록
	printf()	//메세지 출력
	pause()		//signal이 발생할 때까지 대기
*/
int
main()
{
	//&ThreadId[0] : 생성한 thread의id
		//NULL : thread의 attribute
		//Thread1 : thread에서 실행할 함수
		//NULL : Thread1함수의 파라미터 없음
		//Thread1()를 실행하는 thread 생성
	if (pthread_create(&ThreadId[0], NULL, (void*)Thread1, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//&ThreadIdd2 : 생성한 thread의id
		//NULL : thread의 attribute
		//Thread2 : thread에서 실행할 함수
		//NULL : Thread2함수의 파라미터 없음
		//Thread2()를 실행하는 thread 생성
	if (pthread_create(&ThreadId[1], NULL, (void*)Thread2, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//SIGINT signal handler 등록
	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit\n");

	for (; ; )
		pause();	//signal이 발생할 때까지 대기
}
