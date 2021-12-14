#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

BoundedBufferType	Buf;
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
	시간차를 두며 circuler queue의 데이터를 읽어옴
		mutex, condition variable을 사용하여 synchronization 구현
[Input]
	void	*dummy
[Output]	: thread 종료까지 몇 개의 데이터를 저장했는지, 현재 데이터가 몇 개인지 출력
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
		//mutex lock
		if (pthread_mutex_lock(&Mutex) < 0) {
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		//버퍼가 찼으면 NotFull condition variable을 기다림+ lock을 잠시 해제
		while (Buf.counter == MAX_BUF) {
			if (pthread_cond_wait(&NotFull, &Mutex) < 0) {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;	//랜덤 데이터 생성
		Buf.buf[Buf.in].data = data;	//enqueue
		Buf.in = (Buf.in + 1) % MAX_BUF;//in pointer update
		Buf.counter++;			//데이터 개수 증가

		//pthread_cond_wait으로 NotEmpty condition variable을 기다리는 스레드를 깨움
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
	mutex, condition variable을 사용하여 synchronization 구현
[Input]
		void    *dummy
[Output]        : thread 종료까지 몇 개의 데이터를 읽고 현재 데이터가 몇 개인지 출력
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
	BoundedBufferType       Buf	//shared data
[Returns]       : Nothing.
*/
void
Consumer(void* dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	for (i = 0; i < NLOOPS; i++) {
		//mutex lock
		if (pthread_mutex_lock(&Mutex) < 0) {
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		//버퍼가 비었으면 NotEmpty condition variable을 기다림+ lock을 잠시 해제
		while (Buf.counter == 0) {
			if (pthread_cond_wait(&NotEmpty, &Mutex) < 0) {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;		//dequeue
		Buf.out = (Buf.out + 1) % MAX_BUF;	//out pointer update
		Buf.counter--;				//데이터 개수 감소

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
	}

	//몇개의 데이터를 읽었는지, 현재 몇개의 데이터가 존재하는지 출력
	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);	//thread 종료
}

/*
[Program Name]	: circular queue multithread program
[Description]	: mutex, condition variable을 이용한 동기화 멀티스레드 프로그램
[Input]		: Nothing.
[Output]	: 현재 버퍼의 데이터 개수 출력
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
	//현재 버퍼에 있는 데이터 개수 출력
	printf("Main    : %d items in buffer.....\n", Buf.counter);

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
