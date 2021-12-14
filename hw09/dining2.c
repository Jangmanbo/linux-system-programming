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
[Description]	: �����带 ���� �ð����� sleep
[Input]
		int	usecs	//sleep�� �ð�
[Output]        : Nothing.
[Call By]       : Producer, Consumer
[Calls]
		printf()        //�޼��� ���
		perror()        //���� �޼��� ���
		pthread_exit()  //thread ����
	pthread_cond_init()	//condition variable ����
	pthread_mutex_init()	//mutex ����
	gettimeofday()		//���� �ð� ������
	thread_cond_timedwait()	//����ð����� ts���� condition variable�� ��ٸ�
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

	//condition variable ����
	if (pthread_cond_init(&cond, NULL) < 0) {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	//mutex ����
	if (pthread_mutex_init(&mutex, NULL) < 0) {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);	//���� �ð� ������
	ts.tv_sec = tv.tv_sec + usecs / 1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000) {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	//critical section�� ��ȣ�ϱ� ���� mutex lock
	if (pthread_mutex_lock(&mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	//����ð����� ts���� condition variable�� ��ٸ�
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

//ThreadUsleep �Լ��� ȣ���Ͽ� ���� �ð����� ���
void
Thinking(int id)
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand() % 200) * 10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

//ThreadUsleep �Լ��� ȣ���Ͽ� ���� �ð����� ���
void
Eating(int id)
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand() % 100) * 10000);
}

/*
[Function Name] : Test
[Description]   : �Ļ縦 �� �� �ִ��� �׽�Ʈ
[Input]
		int    id       //�ڽ��� ���° ö��������
[Output]        :
[Call By]       : DiningPhilosopher
[Calls]
		sem_post()      //semaphore ���� 1 ����, �ش� semaphore�� ��ٸ��� �����带 ����
[Given]
		int     State[] //�� ö������ ����
		sem_t   Philosoper[]    //�� ö������ �Ļ� ���� ����
[Returns]       : Nothing.
*/
void
Test(int id)
{
	//ö���ڰ� ������� ���� ������ ö���ڰ� ���� �ʰ� �ִٸ� ���� ������ ��� ��� �����ϹǷ� ���¸� EATING���� ����
	if (State[id] == HUNGRY && State[LEFT(id)] != EATING
		&& State[RIGHT(id)] != EATING) {
		State[id] = EATING;

		//ö���ڰ� �Ļ� ���� �������� �˸�
		//Philosopher semaphore�� �� 1 ����
		if (sem_post(&Philosopher[id]) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

/*
[Function Name] : PickUp
[Description]
	���� �������� ����Ͽ� �Ļ�
	�Ļ��� �� ���� ���¸� �� �� ���� ������ ���
[Input]
		int    id       //�ڽ��� ���° ö��������
[Output]        : Nothing.
[Call By]       : DiningPhilosopher
[Calls]
		perror()        //���� �޼��� ���
		phthread_exit() //������ ����
		sem_wait()      //semaphore ���� 1 �̻��̸� ����, �ƴϸ� ��ٸ�
		sem_post()      //semaphore ���� 1 ����, �ش� semaphore�� ��ٸ��� �����带 ����
		Test()          //
[Given]
	sem_t	Mutex	//critical section ��ȣ
	int	State[]	//�� ö������ ����
	sem_t	Philosoper[]	//�� ö������ �Ļ� ���� ����
[Returns]       : Nothing.
*/
void
PickUp(int id)
{
	//Mutex semaphore�� ��ٸ�
		//�ٸ� �����尡 critical section ���� ������ �������� ������ ��ٸ�
	if (sem_wait(&Mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = HUNGRY;

	//�Ļ��� �� �ִ��� �׽�Ʈ
	Test(id);

	//Mutex semaphore �ݳ�
		//critical section�� ��������
	if (sem_post(&Mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	//ö���ڰ� �Ļ�
	//Philosopher semaphore�� �� 1 ����
	if (sem_wait(&Philosopher[id]) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}
}

/*
[Function Name] : PutDown
[Description]	: ���� ������ �ݳ�
[Input]
		int    id	//�ڽ��� ���° ö��������
[Output]        :
[Call By]       : DiningPhilosopher
[Calls]
	perror()	//���� �޼��� ���
	phthread_exit()	//������ ����
	sem_wait()	//semaphore ���� 1 �̻��̸� ����, �ƴϸ� ��ٸ�
	sem_post()	//semaphore ���� 1 ����, �ش� semaphore�� ��ٸ��� �����带 ����
	Test()		//
[Given]
	sem_t   Mutex   //critical section ��ȣ
		int     State[] //�� ö������ ����
[Returns]       : Nothing.
*/
void
PutDown(int id)
{
	//Mutex semaphore�� ��ٸ�
	//�ٸ� �����尡 critical section ���� ������ �������� ������ ��ٸ�
	if (sem_wait(&Mutex) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	//�������� �ݳ��ϹǷ� state�� THINKING���� ����
	State[id] = THINKING;

	//�¿쿡 �ִ� ö���ڰ� ���� �� �ִ��� �׽�Ʈ
	Test(LEFT(id));
	Test(RIGHT(id));

	//Mutex semaphore �ݳ�
	//critical section�� ��������
	if (sem_post(&Mutex) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
}

/*
[Function Name] : DiningPhilosopher Thread
[Description]
	���� �������� �� ���� ���� dead lock�� �߻����� ����
	�� ���� �������� �ݳ�
[Input]
		int    *pId	//�ڽ��� ���° ö��������
[Output]        : �ڽ��� ��� �Ļ��ߴ��� ���
[Call By]       : main
[Calls]
	printf()	//�޼��� ���
	pthread_exit()	//thread ����
	Thinking()	//���� �ð����� sleep
	Eating()	//���� �ð����� sleep
	PickUp()	//�������� �� ���� ����
	PutDown()	//�������� �� ���� �ݳ�
[Given]		: Nothing.
[Returns]       : Nothing.
*/
void
DiningPhilosopher(int* pId)
{
	int		i;
	int		id = *pId;

	for (i = 0; i < NLOOPS; i++) {

		Thinking(id);	//���� �ð����� sleep

		PickUp(id);	//�������� �� ���� ����

		Eating(id);	//���� �ð����� sleep

		PutDown(id);	//�������� �� ���� �ݳ�
	}

	//�ڽ��� ��� �Ļ��ߴ��� ���
	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);	//������ ����
}

/*
[Program Name]  : mutex semaphore thread program
[Description]
	��Դ� ö���� ���α׷�
	critical section�� ��ȣ, hold and wait�� �����Ͽ� dead lock�� �߻����� ����
[Input]		: Nothing.
[Output]	: Nothing.
[Calls]         :
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	srand()		//���� ����
	sem_init()	//semaphore ����
	sem_destroy()	//semaphore ����
	pthread_create()//thread ����
	pthread_join()	//thread�� ���Ḧ ��ٸ�
*/
int
main()
{
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);

	for (i = 0; i < NUM_MEN; i++) {
		//ö���ں��� semaphore ����
		if (sem_init(&Philosopher[i], 0, 0) < 0) {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}
	//mutex semaphore ����(critical section ��ȣ)
	if (sem_init(&Mutex, 0, 1) < 0) {
		perror("sem_init");
		exit(1);
	}

	for (i = 0; i < NUM_MEN; i++) {
		//�� ö������ ������ ����
		if (pthread_create(&tid[i], NULL, (void*)DiningPhilosopher,
			(void*)&id[i]) < 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	for (i = 0; i < NUM_MEN; i++) {
		//��� �����尡 ����Ǳ⸦ ��ٸ�
		if (pthread_join(tid[i], NULL) < 0) {
			perror("pthread_join");
			exit(1);
		}
	}

	//��� semaphore ����
	for (i = 0; i < NUM_MEN; i++) {
		if (sem_destroy(&Philosopher[i]) < 0) {
			perror("sem_destroy");
		}
	}
}
