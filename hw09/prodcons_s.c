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

/*
[Function Name]	: Producer thread
[Description]
	�ð����� �θ� circuler queue�� �����͸� �о��
	mutex, condition variable�� ���� semaphore�� �̿��Ͽ� synchronization ����
[Input]
	void	*dummy
[Output]	: thread ������� �� ���� �����͸� �����ߴ���, ���� �����Ͱ� �� ������ ���
[Call By]	: main
[Calls]
	printf()        //�޼��� ���
		perror()        //���� �޼��� ���
		ThreadUsleep()  //���� �ð����� ���
		sem_wait()      //semaphore�� ��ٸ�,  ���� 1 ����
		sem_post()      //semaphore�� ��ٸ��� thread�� ����, ���� 1 ����
[Given]
		BoundedBufferType       Buf;                            //������ ������ ����
		sem_t                   EmptySem, FullSem, MutexSem;    //semaphore
[Returns]       : Nothing.
*/
void
Producer(void* dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	for (i = 0; i < NLOOPS; i++) {
		//Producer thread�̹Ƿ� EmptySem semaphore�� ��ٸ�
				//�ٸ� ������κ��� semaphore�� �ݳ��Ǹ� ���� 1 ����
		if (sem_wait(&EmptySem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		//critical section�̹Ƿ� MutexSem semaphore�� ��ٸ�
				//�ٸ� ������κ��� semaphore�� �ݳ��Ǹ� ���� 1 ����
		if (sem_wait(&MutexSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;
		Buf.buf[Buf.in].data = data;	//enqueue
		Buf.in = (Buf.in + 1) % MAX_BUF;//in counter update
		Buf.counter++;			//������ ���� ����

		//critical section ��������
				//MutexSem semaphore�� ��ٸ��� thread�� ����
				//MutexSem semaphore�� �� 1 ����
		if (sem_post(&MutexSem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}
		//FullSem semaphore�� ��ٸ��� thread�� ����
				//FullSem semaphore�� �� 1 ����
		if (sem_post(&FullSem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep(data);	//���� �ð����� sleep
	}

	//�� ���� �����͸� �����ߴ���, ���� �� ���� �����Ͱ� �����ϴ��� ���
	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);	//thread ����
}

/*
[Function Name] : Consumer thread
[Description]
	�ð����� �θ� circuler queue�� �����͸� �о��
	mutex, condition variable�� ���� semaphore�� �̿��Ͽ� synchronization ����
[Input]
		void    *dummy
[Output]        : thread ������� �� ���� �����͸� �а� ���� �����Ͱ� �� ������ ���
[Call By]       : main
[Calls]
	printf()	//�޼��� ���
	perror()	//���� �޼��� ���
	ThreadUsleep()	//���� �ð����� ���
	sem_wait()	//semaphore�� ��ٸ�,  ���� 1 ����
	sem_post()	//semaphore�� ��ٸ��� thread�� ����, ���� 1 ����
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
		//Counsumer thread�̹Ƿ� FullSem semaphore�� ��ٸ�
		//�ٸ� ������κ��� semaphore�� �ݳ��Ǹ� ���� 1 ����
		if (sem_wait(&FullSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}
		//critical section�̹Ƿ� MutexSem semaphore�� ��ٸ�
		//�ٸ� ������κ��� semaphore�� �ݳ��Ǹ� ���� 1 ����
		if (sem_wait(&MutexSem) < 0) {
			perror("sem_wait");
			pthread_exit(NULL);
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;		//dequeue
		Buf.out = (Buf.out + 1) % MAX_BUF;	//out pointer udpate
		Buf.counter--;				//������ ���� ����

		//critical section ��������
		//MutexSem semaphore�� ��ٸ��� thread�� ����
		//MutexSem semaphore�� �� 1 ����
		if (sem_post(&MutexSem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		//EmptySem semaphore�� ��ٸ��� thread�� ����
				//EmptySem semaphore�� �� 1 ����
		if (sem_post(&EmptySem) < 0) {
			perror("sem_post");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand() % 100) * 10000);	//���� �ð����� 
	}

	//��� �����͸� �о�����, ���� ��� �����Ͱ� �����ϴ��� ���
	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);	//thread ����
}

/*
[Program Name]	: circular queue multithread program
[Description]	: mutex, condition variable�� ���� semaphore�� �̿���  ����ȭ ��Ƽ������ ���α׷�
[Input]		: Nothing.
[Output]	: ���� ������ ������ ���� ���
[Calls]
	sem_init()		//semaphore ����
	sem_destroy()		//semaphore ����
	pthread_create()	//thread ����
	pthread_join()		//thread ���Ḧ ��ٸ�
	srand()			//���� ����
	perror()		//���� �޼��� ���
	exit()			//���μ��� ����
	printf()		//�޼��� ���
*/
int
main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);	//���� ����

	//EmptySem, FullSem counting semaphore 2�� ����, Mutex binary semaphore 1�� ����
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

	//producer, consumer thread ����
	if (pthread_create(&tid1, NULL, (void*)Producer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void*)Consumer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//producer, consumer thread�� ���Ḧ ��ٸ�
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	//���� ���ۿ� �����ϴ� �������� ���� ���
	printf("Main    : %d items in buffer.....\n", Buf.counter);

	//semaphore ����
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
