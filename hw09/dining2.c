#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5
#define	NLOOPS		5

#define	LEFT(i)		((i+NUM_MEN-1)%NUM_MEN)
#define	RIGHT(i)	((i+1)%NUM_MEN)

enum { THINKING, EATING, HUNGRY };

sem_t	Philosopher[NUM_MEN];
sem_t	Mutex;
int		State[NUM_MEN];

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
[Function Name] : Test
[Description]   : 식사를 할 수 있는지 테스트
[Input]
		int    id       //자신이 몇번째 철학자인지
[Output]        :
[Call By]       : DiningPhilosopher
[Calls]
		sem_post()      //semaphore 값을 1 증가, 해당 semaphore를 기다리는 스레드를 깨움
[Given]
		int     State[] //각 철학자의 상태
		sem_t   Philosoper[]    //각 철학자의 식사 가능 여부
[Returns]       : Nothing.
*/
void
Test(int id)
{
	//철학자가 배고프고 왼쪽 오른쪽 철학자가 먹지 않고 있다면 양쪽 젓가락 모두 사용 가능하므로 상태를 EATING으로 변경
	if (State[id] == HUNGRY && State[LEFT(id)] != EATING
		&& State[RIGHT(id)] != EATING) {
		State[id] = EATING;

		//철학자가 식사 가능 상태임을 알림
		//Philosopher semaphore의 값 1 증가
		if (sem_post(&Philosopher[id]) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

/*
[Function Name] : PickUp
[Description]
	양쪽 젓가락을 사용하여 식사
	식사할 수 없는 상태면 할 수 있을 때까지 대기
[Input]
		int    id       //자신이 몇번째 철학자인지
[Output]        : Nothing.
[Call By]       : DiningPhilosopher
[Calls]
		perror()        //에러 메세지 출력
		phthread_exit() //스레드 종료
		sem_wait()      //semaphore 값이 1 이상이면 감소, 아니면 기다림
		sem_post()      //semaphore 값을 1 증가, 해당 semaphore를 기다리는 스레드를 깨움
		Test()          //
[Given]
	sem_t	Mutex	//critical section 보호
	int	State[]	//각 철학자의 상태
	sem_t	Philosoper[]	//각 철학자의 식사 가능 여부
[Returns]       : Nothing.
*/
void
PickUp(int id)
{
	//Mutex semaphore를 기다림
		//다른 스레드가 critical section 내에 있으면 빠져나올 때까지 기다림
	if (sem_wait(&Mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = HUNGRY;

	//식사할 수 있는지 테스트
	Test(id);

	//Mutex semaphore 반납
		//critical section을 빠져나옴
	if (sem_post(&Mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	//철학자가 식사
	//Philosopher semaphore의 값 1 감소
	if (sem_wait(&Philosopher[id]) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}
}

/*
[Function Name] : PutDown
[Description]	: 양쪽 젓가락 반납
[Input]
		int    id	//자신이 몇번째 철학자인지
[Output]        :
[Call By]       : DiningPhilosopher
[Calls]
	perror()	//에러 메세지 출력
	phthread_exit()	//스레드 종료
	sem_wait()	//semaphore 값이 1 이상이면 감소, 아니면 기다림
	sem_post()	//semaphore 값을 1 증가, 해당 semaphore를 기다리는 스레드를 깨움
	Test()		//
[Given]
	sem_t   Mutex   //critical section 보호
		int     State[] //각 철학자의 상태
[Returns]       : Nothing.
*/
void
PutDown(int id)
{
	//Mutex semaphore를 기다림
	//다른 스레드가 critical section 내에 있으면 빠져나올 때까지 기다림
	if (sem_wait(&Mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	//젓가락을 반납하므로 state를 THINKING으로 변경
	State[id] = THINKING;

	//좌우에 있는 철학자가 먹을 수 있는지 테스트
	Test(LEFT(id));
	Test(RIGHT(id));

	//Mutex semaphore 반납
	//critical section을 빠져나옴
	if (sem_post(&Mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
}

/*
[Function Name] : DiningPhilosopher Thread
[Description]
	양쪽 젓가락을 한 번에 집어 dead lock이 발생하지 않음
	한 번에 젓가락을 반납
[Input]
		int    *pId	//자신이 몇번째 철학자인지
[Output]        : 자신이 몇번 식사했는지 출력
[Call By]       : main
[Calls]
	printf()	//메세지 출력
	pthread_exit()	//thread 종료
	Thinking()	//랜덤 시간동안 sleep
	Eating()	//랜덤 시간동안 sleep
	PickUp()	//젓가락을 한 번에 잡음
	PutDown()	//젓가락을 한 번에 반납
[Given]		: Nothing.
[Returns]       : Nothing.
*/
void
DiningPhilosopher(int* pId)
{
	int		i;
	int		id = *pId;

	for (i = 0; i < NLOOPS; i++) {

		Thinking(id);	//랜덤 시간동안 sleep

		PickUp(id);	//젓가락을 한 번에 잡음

		Eating(id);	//랜덤 시간동안 sleep

		PutDown(id);	//젓가락을 한 번에 반납
	}

	//자신이 몇번 식사했는지 출력
	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);	//스레드 종료
}

/*
[Program Name]  : mutex semaphore thread program
[Description]
	밥먹는 철학자 프로그램
	critical section을 보호, hold and wait을 방지하여 dead lock이 발생하지 않음
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

	srand(0x8888);

	for (i = 0; i < NUM_MEN; i++) {
		//철학자별로 semaphore 생성
		if (sem_init(&Philosopher[i], 0, 0) < 0) {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}
	//mutex semaphore 생성(critical section 보호)
	if (sem_init(&Mutex, 0, 1) < 0) {
		perror("sem_init");
		exit(1);
	}

	for (i = 0; i < NUM_MEN; i++) {
		//각 철학자의 스레드 생성
		if (pthread_create(&tid[i], NULL, (void*)DiningPhilosopher,
			(void*)&id[i]) < 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	for (i = 0; i < NUM_MEN; i++) {
		//모든 스레드가 종료되기를 기다림
		if (pthread_join(tid[i], NULL) < 0) {
			perror("pthread_join");
			exit(1);
		}
	}

	//모든 semaphore 삭제
	for (i = 0; i < NUM_MEN; i++) {
		if (sem_destroy(&Philosopher[i]) < 0) {
			perror("sem_destroy");
		}
	}
}
