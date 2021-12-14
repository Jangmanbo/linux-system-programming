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
		mutex, condition variable�� ����Ͽ� synchronization ����
[Input]
	void	*dummy
[Output]	: thread ������� �� ���� �����͸� �����ߴ���, ���� �����Ͱ� �� ������ ���
[Call By]	: main
[Calls]
	printf()        //�޼��� ���
		perror()        //���� �޼��� ���
		ThreadUsleep()  //���� �ð����� ���
		pthread_exit()  //thread ����
		pthread_mutex_lock()    //mutex lock
		pthread_cond_wait()     //condition variable�� ��ٸ�
		pthread_cond_signal()   //condition variable��pthread_cond_wait���� ��ٸ��� �����带 ����
		pthread_mutex_unlock()  //mutex unlcok
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
		//mutex lock
		if (pthread_mutex_lock(&Mutex) < 0) {
			perror("pthread_mutex_lock");
			pthread_exit(NULL);
		}

		//���۰� á���� NotFull condition variable�� ��ٸ�+ lock�� ��� ����
		while (Buf.counter == MAX_BUF) {
			if (pthread_cond_wait(&NotFull, &Mutex) < 0) {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;	//���� ������ ����
		Buf.buf[Buf.in].data = data;	//enqueue
		Buf.in = (Buf.in + 1) % MAX_BUF;//in pointer update
		Buf.counter++;			//������ ���� ����

		//pthread_cond_wait���� NotEmpty condition variable�� ��ٸ��� �����带 ����
		if (pthread_cond_signal(&NotEmpty) < 0) {
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}

		//mutex unlock
		if (pthread_mutex_unlock(&Mutex) < 0) {
			perror("pthread_mutex_unlock");
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
	mutex, condition variable�� ����Ͽ� synchronization ����
[Input]
		void    *dummy
[Output]        : thread ������� �� ���� �����͸� �а� ���� �����Ͱ� �� ������ ���
[Call By]       : main
[Calls]
	printf()	//�޼��� ���
	perror()	//���� �޼��� ���
	ThreadUsleep()	//���� �ð����� ���
	pthread_exit()	//thread ����
	pthread_mutex_lock()	//mutex lock
	pthread_cond_wait()	//condition variable�� ��ٸ�
	pthread_cond_signal()	//condition variable��pthread_cond_wait���� ��ٸ��� �����带 ����
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

		//���۰� ������� NotEmpty condition variable�� ��ٸ�+ lock�� ��� ����
		while (Buf.counter == 0) {
			if (pthread_cond_wait(&NotEmpty, &Mutex) < 0) {
				perror("pthread_cond_wait");
				pthread_exit(NULL);
			}
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;		//dequeue
		Buf.out = (Buf.out + 1) % MAX_BUF;	//out pointer update
		Buf.counter--;				//������ ���� ����

		//NotFull condition variable��pthread_cond_wait���� ��ٸ��� �����带 ����
		if (pthread_cond_signal(&NotFull) < 0) {
			perror("pthread_cond_signal");
			pthread_exit(NULL);
		}

		//mutex unlock
		if (pthread_mutex_unlock(&Mutex) < 0) {
			perror("pthread_mutex_unlock");
			pthread_exit(NULL);
		}

		ThreadUsleep((rand() % 100) * 10000);	//���� �ð����� sleep
	}

	//��� �����͸� �о�����, ���� ��� �����Ͱ� �����ϴ��� ���
	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	pthread_exit(NULL);	//thread ����
}

/*
[Program Name]	: circular queue multithread program
[Description]	: mutex, condition variable�� �̿��� ����ȭ ��Ƽ������ ���α׷�
[Input]		: Nothing.
[Output]	: ���� ������ ������ ���� ���
[Calls]
	pthread_cond_init()	//condition variable ����
	pthread_mutex_init()	//mutex ����
	pthread_cond_destroy()     //condition variable ����
		pthread_mutex_destroy()    //mutex ����
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

	srand(0x8888);

	//NotFull, NotEmpty condition variable�� mutex ����
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

	//producer, consumer thread ����
	if (pthread_create(&tid1, NULL, (void*)Producer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void*)Consumer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//producer, consumer thread�� ����Ǳ⸦ ��ٸ�
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	//���� ���ۿ� �ִ� ������ ���� ���
	printf("Main    : %d items in buffer.....\n", Buf.counter);

	//condition variable, mutex ����
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
