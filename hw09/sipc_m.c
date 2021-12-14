#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "prodcons.h"

char	pData[100];
int 	count = 0;
pthread_cond_t		NotFull;
pthread_cond_t		NotEmpty;
pthread_mutex_t		Mutex;

/*
[Function Name] : ThreadUsleep
[Description]	: 스레드를 일정 시간동안 sleep
[Input]
		int	usecs	//sleep할 시간
[Output]        : Nothing.
[Call By]       : Producer, Consumer
[Calls]
		printf()        //메세지 출력
		perror()        //에러 메세지 출력
		pthread_exit()  //thread 종료
	pthread_cond_init()	//condition variable 생성
	pthread_mutex_init()	//mutex 생성
	gettimeofday()		//현재 시간 가져옴
	thread_cond_timedwait()	//현재시간부터 ts까지 condition variable을 기다림
	pthread_cond_destroy()	//destroy condition variable
	pthread_mutex_destroy()	//destory mutex
[Given]		: Nothing.
[Returns]       : Nothing.
*/
void
ThreadUsleep(int usecs)
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	//condition variable 생성
	if (pthread_cond_init(&cond, NULL) < 0) {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	//mutex 생성
	if (pthread_mutex_init(&mutex, NULL) < 0) {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);	//현재 시간 가져옴
	ts.tv_sec = tv.tv_sec + usecs / 1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000) {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	//critical section을 보호하기 위한 mutex lock
	if (pthread_mutex_lock(&mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	//현재시간부터 ts까지 condition variable을 기다림
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0) {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	//destroy condition variable, mutex
	if (pthread_cond_destroy(&cond) < 0) {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0) {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

/*
[Function Name]	: Producer thread
[Description]
	데이터를 저장
		mutex, condition variable을 사용하여 synchronization 구현
[Input]
	void	*dummy
[Output]	: Nothing.
[Call By]	: main
[Calls]
	printf()        //메세지 출력
		perror()        //에러 메세지 출력
		ThreadUsleep()  //랜덤 시간동안 대기
		pthread_exit()  //thread 종료
		pthread_mutex_lock()    //mutex lock
		pthread_cond_wait()     //condition variable을 기다림
		pthread_cond_signal()   //condition variable을pthread_cond_wait으로 기다리는 스레드를 깨움
		pthread_mutex_unlock()  //mutex unlcok
[Given]
	phtread_cond_t  NotFull, NotEmpty, Mutex;
[Returns]       : Nothing.
*/
void
Producer(void* dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	//mutex lock
	if (pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	//데이터가 이미 있으면
	while (count == 1) {
		//NotFull conditino variable을 기다림
		if (pthread_cond_wait(&NotFull, &Mutex) < 0) {
			perror("pthread_cond_wait");
			pthread_exit(NULL);
		}
	}
	
	sprintf(pData, "This is a request from %ld.", pthread_self());
	printf("Sent a request.....\n");

	count++;	//데이터를 저장했으므로 데이터 개수 증가

	//NotEmpty condition variable을pthread_cond_wait으로 기다리는 스레드를 깨움
	if (pthread_cond_signal(&NotEmpty) < 0) {
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}

	//mutex unlock
	if (pthread_mutex_unlock(&Mutex) < 0) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	ThreadUsleep(data);	//랜덤 시간동안 sleep

	printf("Received reply : %s\n", pData);

	pthread_exit(NULL);	//thread 종료
}

/*
[Function Name] : Consumer thread
[Description]
	데이터를 읽어옴
	mutex, condition variable을 사용하여 synchronization 구현
[Input]
		void    *dummy
[Output]        : Nothing.
[Call By]       : main
[Calls]
	printf()	//메세지 출력
	perror()	//에러 메세지 출력
	ThreadUsleep()	//랜덤 시간동안 대기
	pthread_exit()	//thread 종료
	pthread_mutex_lock()	//mutex lock
	pthread_cond_wait()	//condition variable을 기다림
	pthread_cond_signal()	//condition variable을pthread_cond_wait으로 기다리는 스레드를 깨움
	pthread_mutex_unlock()	//mutex unlcok
[Given]
	phtread_cond_t	NotFull, NotEmpty, Mutex;
[Returns]       : Nothing.
*/
void
Consumer(void* dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	//mutex lock
	if (pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	//읽을 데이터가 없으면
	while (count == 0) {
		//NotEmpty condition varaible을 기다림
		if (pthread_cond_wait(&NotEmpty, &Mutex) < 0) {
			perror("pthread_cond_wait");
			pthread_exit(NULL);
		}
	}

	printf("Received request: %s.....\n", pData);
	sprintf(pData, "This is a reply from %ld.", pthread_self());

	//읽었으므로 데이터 감소
	count--;

	//NotFull condition variable을pthread_cond_wait으로 기다리는 스레드를 깨움
	if (pthread_cond_signal(&NotFull) < 0) {
		perror("pthread_cond_signal");
		pthread_exit(NULL);
	}

	//mutex unlock
	if (pthread_mutex_unlock(&Mutex) < 0) {
		perror("pthread_mutex_unlock");
		pthread_exit(NULL);
	}

	ThreadUsleep((rand() % 100) * 10000);	//랜덤 시간동안 sleep

	pthread_exit(NULL);	//thread 종료
}

/*
[Program Name]	: sipc1.c, sipc2.c mutex&condition variable program
[Description]	: sipc1.c, sipc2.c를 mutex, condition variable을 이용한 동기화 멀티스레드 프로그램으로 변경
[Input]		: Nothing.
[Output]	: Nothing.
[Calls]
	pthread_cond_init()	//condition variable 생성
	pthread_mutex_init()	//mutex 생성
	pthread_cond_destroy()     //condition variable 삭제
		pthread_mutex_destroy()    //mutex 삭제
	pthread_create()	//thread 생성
	pthread_join()		//thread 종료를 기다림
	srand()			//난수 생성
	perror()		//에러 메세지 출력
	exit()			//프로세스 종료
	printf()		//메세지 출력
*/
/*
	semaphore과 mutex&condition variable의 차이
	semaphore : 값을 증가한 다음 감소시키면 기다리고 있던 스레드가 signal을 받음
	mutex&condition variable은 wait을 해도 signal을 받지 못함
*/
int
main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	//NotFull, NotEmpty condition variable과 mutex 생성
	if (pthread_cond_init(&NotFull, NULL) < 0) {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_cond_init(&NotEmpty, NULL) < 0) {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&Mutex, NULL) < 0) {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	//producer, consumer thread 생성
	if (pthread_create(&tid1, NULL, (void*)Producer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void*)Consumer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//producer, consumer thread가 종료되기를 기다림
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	//condition variable, mutex 삭제
	if (pthread_cond_destroy(&NotFull) < 0) {
		perror("pthread_cond_destroy");
	}
	if (pthread_cond_destroy(&NotEmpty) < 0) {
		perror("pthread_cond_destroy");
	}
	if (pthread_mutex_destroy(&Mutex) < 0) {
		perror("pthread_mutex_destroy");
	}
}
