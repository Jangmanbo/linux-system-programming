#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "semlib2.h"
#include "prodcons.h"

BoundedBufferType	Buf;
sem_t				EmptySem, FullSem, MutexSem;

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
	시간차를 두며 circuler queue의 데이터를 읽어옴
	mutex, condition variable로 만든 semaphore를 이용하여 synchronization 구현
[Input]
	void	*dummy
[Output]	: thread 종료까지 몇 개의 데이터를 저장했는지, 현재 데이터가 몇 개인지 출력
[Call By]	: main
[Calls]
	printf()        //메세지 출력
		perror()        //에러 메세지 출력
		ThreadUsleep()  //랜덤 시간동안 대기
		sem_wait()      //semaphore를 기다림,  값을 1 감소
		sem_post()      //semaphore를 기다리는 thread를 깨움, 값을 1 증가
[Given]
		BoundedBufferType       Buf;                            //데이터 저장할 버퍼
		sem_t                   EmptySem, FullSem, MutexSem;    //semaphore
[Returns]       : Nothing.
*/
void
Producer(void* dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	for (i = 0; i < NLOOPS; i++) {
		//Producer thread이므로 EmptySem semaphore를 기다림
				//다른 스레드로부터 semaphore가 반납되면 값을 1 감소
		if (sem_wait(&EmptySem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		//critical section이므로 MutexSem semaphore를 기다림
				//다른 스레드로부터 semaphore가 반납되면 값을 1 감소
		if (sem_wait(&MutexSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;
		Buf.buf[Buf.in].data = data;	//enqueue
		Buf.in = (Buf.in + 1) % MAX_BUF;//in counter update
		Buf.counter++;			//데이터 개수 증가

		//critical section 빠져나옴
				//MutexSem semaphore를 기다리는 thread를 깨움
				//MutexSem semaphore의 값 1 감소
		if (sem_post(&MutexSem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}
		//FullSem semaphore를 기다리는 thread를 깨움
				//FullSem semaphore의 값 1 감소
		if (sem_post(&FullSem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);	//랜덤 시간동안 sleep
	}

	//몇 개의 데이터를 저장했는지, 현재 몇 개의 데이터가 존재하는지 출력
	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);	//thread 종료
}

/*
[Function Name] : Consumer thread
[Description]
	시간차를 두며 circuler queue의 데이터를 읽어옴
	mutex, condition variable로 만든 semaphore를 이용하여 synchronization 구현
[Input]
		void    *dummy
[Output]        : thread 종료까지 몇 개의 데이터를 읽고 현재 데이터가 몇 개인지 출력
[Call By]       : main
[Calls]
	printf()	//메세지 출력
	perror()	//에러 메세지 출력
	ThreadUsleep()	//랜덤 시간동안 대기
	sem_wait()	//semaphore를 기다림,  값을 1 감소
	sem_post()	//semaphore를 기다리는 thread를 깨움, 값을 1 증가
[Given]
	BoundedBufferType       Buf	//shared data
	sem_t                   EmptySem, FullSem, MutexSem;    //semaphore
[Returns]       : Nothing.
*/
void
Consumer(void* dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	for (i = 0; i < NLOOPS; i++) {
		//Counsumer thread이므로 FullSem semaphore를 기다림
		//다른 스레드로부터 semaphore가 반납되면 값을 1 감소
		if (sem_wait(&FullSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		//critical section이므로 MutexSem semaphore를 기다림
		//다른 스레드로부터 semaphore가 반납되면 값을 1 감소
		if (sem_wait(&MutexSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;		//dequeue
		Buf.out = (Buf.out + 1) % MAX_BUF;	//out pointer udpate
		Buf.counter--;				//데이터 개수 감소

		//critical section 빠져나옴
		//MutexSem semaphore를 기다리는 thread를 깨움
		//MutexSem semaphore의 값 1 감소
		if (sem_post(&MutexSem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		//EmptySem semaphore를 기다리는 thread를 깨움
				//EmptySem semaphore의 값 1 감소
		if (sem_post(&EmptySem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand() % 100) * 10000);	//랜덤 시간동안 
	}

	//몇개의 데이터를 읽었는지, 현재 몇개의 데이터가 존재하는지 출력
	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);	//thread 종료
}

/*
[Program Name]	: circular queue multithread program
[Description]	: mutex, condition variable로 만든 semaphore를 이용한  동기화 멀티스레드 프로그램
[Input]		: Nothing.
[Output]	: 현재 버퍼의 데이터 개수 출력
[Calls]
	sem_init()		//semaphore 생성
	sem_destroy()		//semaphore 삭제
	pthread_create()	//thread 생성
	pthread_join()		//thread 종료를 기다림
	srand()			//난수 생성
	perror()		//에러 메세지 출력
	exit()			//프로세스 종료
	printf()		//메세지 출력
*/
int
main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);	//난수 생성

	//EmptySem, FullSem counting semaphore 2개 생성, Mutex binary semaphore 1개 생성
	if (sem_init(&EmptySem, 0, MAX_BUF) < 0) {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&FullSem, 0, 0) < 0) {
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&MutexSem, 0, 1) < 0) {
		perror("sem_init");
		exit(1);
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

	//producer, consumer thread의 종료를 기다림
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	//현재 버퍼에 존재하는 데이터의 개수 출력
	printf("Main    : %d items in buffer.....\n", Buf.counter);

	//semaphore 삭제
	if (sem_destroy(&EmptySem) < 0) {
		perror("sem_destroy");
	}
	if (sem_destroy(&FullSem) < 0) {
		perror("sem_destroy");
	}
	if (sem_destroy(&MutexSem) < 0) {
		perror("sem_destroy");
	}
}
