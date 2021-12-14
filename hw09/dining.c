#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5
#define	NLOOPS		5

//��Ƽ�������̹Ƿ� ���������� ����
sem_t	ChopStick[NUM_MEN];

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
[Function Name] : DiningPhilosopher Thread
[Description]
	���� �������� ������ �������� ������ ���� �Ļ�
	������ �ٸ� ö���ڰ� �ݳ��� ������ ��ٸ�
	���� ������ ������ �ݳ�
	���� �������� �� ���� ���� �ʱ� ������ dead lock�� �߻��� �� ����
[Input]
		int    *pId	//�ڽ��� ���° ö��������
[Output]        :
[Call By]       : main
[Calls]
	printf()	//�޼��� ���
	perror()	//���� �޼��� ���
	pthread_exit()	//thread ����
	sem_wait()	//semaphore ���� 1 �̻��̸� ����, �ƴϸ� ��ٸ�
	sem_post()	//semaphore ���� 1 ����, �ش� semaphore�� ��ٸ��� �����带 ����
	Thinking()	//���� �ð����� sleep
	Eating()	//���� �ð����� sleep
[Given]
	sem_t	ChopStick[NUM_MEN]	//���Ŷ� semaphore
[Returns]       : Nothing.
*/
void
DiningPhilosopher(int* pId)
{
	int		i;
	int		id = *pId;

	for (i = 0; i < NLOOPS; i++) {

		//ö���ڰ� ����
		//���� �ð����� sleep
		Thinking(id);

		//�ڽ��� ���� �������� ������ ���(semaphore 1 ����), ������ ��ٸ�
		if (sem_wait(&ChopStick[id]) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		//�ڽ��� ������ �������� ������ ���(semaphore 1 ����), ������ ��ٸ�
		if (sem_wait(&ChopStick[(id + 1) % NUM_MEN]) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		//ö���ڰ� ����
		//���� �ð����� sleep
		Eating(id);

		//�ڽ��� ���� �������� �ݳ�(semaphore 1 ����)
		if (sem_post(&ChopStick[id]) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		//�ڽ��� ������ �������� �ݳ�(semaphore 1 ����)
		if (sem_post(&ChopStick[(id + 1) % NUM_MEN]) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	//thread ����
	pthread_exit(NULL);
}

/*
[Program Name]  : semaphore thread program
[Description]   : ��Դ� ö���� ���α׷�, dead lock�� �߻��� �� ����
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

	srand(0x8888);	//���� ����

	for (i = 0; i < NUM_MEN; i++) {
		//���������� semaphore ����
		if (sem_init(&ChopStick[i], 0, 1) < 0) {
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}

	for (i = 0; i < NUM_MEN; i++) {
		//�� ö������ ������ ����
		if (pthread_create(&tid[i], NULL, (void*)DiningPhilosopher,
			(void*)&id[i]) < 0) {
			perror("pthread_create");
			exit(1);
		}
	}

	//��� �����尡 ����Ǳ⸦ ��ٸ�
	for (i = 0; i < NUM_MEN; i++) {
		if (pthread_join(tid[i], NULL) < 0) {
			perror("pthread_join");
			exit(1);
		}
	}

	//��� semaphore ����
	for (i = 0; i < NUM_MEN; i++) {
		if (sem_destroy(&ChopStick[i]) < 0) {
			perror("sem_destroy");
		}
	}
}
