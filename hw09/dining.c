#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5
#define	NLOOPS		5

//멀티스레드이므로 전역변수로 선언
sem_t	ChopStick[NUM_MEN];

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

//ThreadUsleep 함수를 호출하여 랜덤 시간동안 대기
void
Thinking(int id)
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand() % 200) * 10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

//ThreadUsleep 함수를 호출하여 랜덤 시간동안 대기
void
Eating(int id)
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand() % 100) * 10000);
}

/*
[Function Name] : DiningPhilosopher Thread
[Description]
	왼쪽 젓가락과 오른쪽 젓가락을 차레로 집어 식사
	없으면 다른 철학자가 반납할 때까지 기다림
	왼쪽 오른쪽 순으로 반납
	양쪽 젓가락을 한 번에 집지 않기 때문에 dead lock이 발생할 수 있음
[Input]
		int    *pId	//자신이 몇번째 철학자인지
[Output]        :
[Call By]       : main
[Calls]
	printf()	//메세지 출력
	perror()	//에러 메세지 출력
	pthread_exit()	//thread 종료
	sem_wait()	//semaphore 값이 1 이상이면 감소, 아니면 기다림
	sem_post()	//semaphore 값을 1 증가, 해당 semaphore를 기다리는 스레드를 깨움
	Thinking()	//랜덤 시간동안 sleep
	Eating()	//랜덤 시간동안 sleep
[Given]
	sem_t	ChopStick[NUM_MEN]	//젓거락 semaphore
[Returns]       : Nothing.
*/
void
DiningPhilosopher(int* pId)
{
	int		i;
	int		id = *pId;

	for (i = 0; i < NLOOPS; i++) {

		//철학자가 생각
		//랜덤 시간동안 sleep
		Thinking(id);

		//자신의 왼쪽 젓가락이 있으면 사용(semaphore 1 감소), 없으면 기다림
		if (sem_wait(&ChopStick[id]) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		//자신의 오른쪽 젓가락이 있으면 사용(semaphore 1 감소), 없으면 기다림
		if (sem_wait(&ChopStick[(id + 1) % NUM_MEN]) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		//철학자가 먹음
		//랜덤 시간동안 sleep
		Eating(id);

		//자신의 왼쪽 젓가락을 반납(semaphore 1 증가)
		if (sem_post(&ChopStick[id]) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		//자신의 오른쪽 젓가락을 반납(semaphore 1 증가)
		if (sem_post(&ChopStick[(id + 1) % NUM_MEN]) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	//thread 종료
	pthread_exit(NULL);
}

/*
[Program Name]  : semaphore thread program
[Description]   : 밥먹는 철학자 프로그램, dead lock이 발생할 수 있음
[Input]		: Nothing.
[Output]	: Nothing.
[Calls]         :
	exit()		//프로세스 종료
	perror()	//에러 메세지 출력
	srand()		//난수 생성
	sem_init()	//semaphore 생성
	sem_destroy()	//semaphore 삭제
	pthread_create()//thread 생성
	pthread_join()	//thread의 종료를 기다림
*/
int
main()
{
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);	//난수 생성

	for (i = 0; i < NUM_MEN; i++) {
		//젓가락별로 semaphore 생성
		if (sem_init(&ChopStick[i], 0, 1) < 0) {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}

	for (i = 0; i < NUM_MEN; i++) {
		//각 철학자의 스레드 생성
		if (pthread_create(&tid[i], NULL, (void*)DiningPhilosopher,
			(void*)&id[i]) < 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	//모든 스레드가 종료되기를 기다림
	for (i = 0; i < NUM_MEN; i++) {
		if (pthread_join(tid[i], NULL) < 0) {
			perror("pthread_join");
			exit(1);
		}
	}

	//모든 semaphore 삭제
	for (i = 0; i < NUM_MEN; i++) {
		if (sem_destroy(&ChopStick[i]) < 0) {
			perror("sem_destroy");
		}
	}
}
