#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

//멀티스레드이므로 shared data를 전역변수로 선언
//Buf구조체 내의 멤버변수(counter, buf)를 사용
BoundedBufferType	Buf;

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
[Given]
		BoundedBufferType       Buf;                            //데이터 저장할 버퍼
		sem_t                   EmptySem, FullSem, MutexSem;    //semaphore
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
	시간차를 두며 circular queue에 데이터 저장
	queue가 다 차있으면 자리가 날 때까지 busy waiting
[Input]
	void	*dummy	//
[Output]	: thread 종료까지 몇 개의 데이터를 저장했는지, 현재 데이터가 몇 개인지 출력
[Call By]	: main
[Calls]
		printf()        //메세지 출력
		ThreadUsleep()  //랜덤 시간동안 대기
		pthread_exit()  //thread 종료
[Given]
		BoundedBufferType       Buf     //shared data
[Returns]       : Nothing.
*/
void
Producer(void* dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	for (i = 0; i < NLOOPS; i++) {
		if (Buf.counter == MAX_BUF) {	//큐가 다 차있으면
			printf("Producer: Buffer full. Waiting.....\n");
			while (Buf.counter == MAX_BUF)	//큐에 자리가 날 때까지 busy waiting
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;	//랜덤 데이터 생성
		Buf.buf[Buf.in].data = data;	//생성한 데이터 enqueue
		Buf.in = (Buf.in + 1) % MAX_BUF;//in pointer update
		Buf.counter++;			//데이터 개수 증가 

		ThreadUsleep(data);	//랜덤 시간동안 대기
	}

	//몇개의 데이터를 생성하고 현재 queue에 데이터가 몇개인지 출력
	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);

	//thread 종료
	pthread_exit(NULL);
}

/*
[Function Name] : Consumerer thread
[Description]
	시간차를 두며 circuler queue의 데이터를 읽어옴
	읽을 데이터가 없으면 읽을 때까지busy waiting
[Input]
		void    *dummy
[Output]        : thread 종료까지 몇 개의 데이터를 읽고 현재 데이터가 몇 개인지 출력
[Call By]       : main
[Calls]
	printf()	//메세지 출력
	ThreadUsleep()	//랜덤 시간동안 대기
	pthread_exit()	//thread 종료
[Given]
	BoundedBufferType       Buf	//shared data
[Returns]       : Nothing.
*/
void
Consumer(void* dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	for (i = 0; i < NLOOPS; i++) {
		if (Buf.counter == 0) {	//읽을 데이터가 없으면
			printf("Consumer: Buffer empty. Waiting.....\n");
			while (Buf.counter == 0)//busy waiting
				;
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;		//dequeue
		Buf.out = (Buf.out + 1) % MAX_BUF;	//out pointer update
		Buf.counter--;				//읽었으므로 데이터 개수 감소

		ThreadUsleep((rand() % 100) * 10000);	//랜덤 시간동안 대기
	}

	//데이터를 몇개 읽었고 현재 queue에 데이터가 몇개인지
	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	//thread 종료
	pthread_exit(NULL);
}

/*
[Program Name]	: circular queue multithread program
[Description]	: 두 스레드가 shared data에 동시에 접근가능하며 busy waiting(CPU낭비)
[Input]		: Nothing.
[Output]	:
[Calls]
	pthread_create()	//thread 생성
	pthread_join()		//thread 종료를 기다림
	srand()			//?
	perror()		//에러 메세지 출력
	exit()			//프로세스 종료
	printf()		//메세지 출력
*/
int
main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	//producer thread 생성
	if (pthread_create(&tid1, NULL, (void*)Producer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	//consumer thread 생성
	if (pthread_create(&tid2, NULL, (void*)Consumer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//producer thread의 종료를 기다림
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	//consumer thread의 종료를 기다림
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	//데이터 개수 출력
	printf("Main    : %d items in buffer.....\n", Buf.counter);
}
