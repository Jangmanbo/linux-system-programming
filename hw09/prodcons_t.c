#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "prodcons.h"

//��Ƽ�������̹Ƿ� shared data�� ���������� ����
//Buf����ü ���� �������(counter, buf)�� ���
BoundedBufferType	Buf;

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
[Given]
		BoundedBufferType       Buf;                            //������ ������ ����
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
	�ð����� �θ� circular queue�� ������ ����
	queue�� �� �������� �ڸ��� �� ������ busy waiting
[Input]
	void	*dummy	//
[Output]	: thread ������� �� ���� �����͸� �����ߴ���, ���� �����Ͱ� �� ������ ���
[Call By]	: main
[Calls]
		printf()        //�޼��� ���
		ThreadUsleep()  //���� �ð����� ���
		pthread_exit()  //thread ����
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
		if (Buf.counter == MAX_BUF) {	//ť�� �� ��������
			printf("Producer: Buffer full. Waiting.....\n");
			while (Buf.counter == MAX_BUF)	//ť�� �ڸ��� �� ������ busy waiting
				;
		}

		printf("Producer: Producing an item.....\n");
		data = (rand() % 100) * 10000;	//���� ������ ����
		Buf.buf[Buf.in].data = data;	//������ ������ enqueue
		Buf.in = (Buf.in + 1) % MAX_BUF;//in pointer update
		Buf.counter++;			//������ ���� ���� 

		ThreadUsleep(data);	//���� �ð����� ���
	}

	//��� �����͸� �����ϰ� ���� queue�� �����Ͱ� ����� ���
	printf("Producer: Produced %d items.....\n", i);
	printf("Producer: %d items in buffer.....\n", Buf.counter);

	//thread ����
	pthread_exit(NULL);
}

/*
[Function Name] : Consumerer thread
[Description]
	�ð����� �θ� circuler queue�� �����͸� �о��
	���� �����Ͱ� ������ ���� ������busy waiting
[Input]
		void    *dummy
[Output]        : thread ������� �� ���� �����͸� �а� ���� �����Ͱ� �� ������ ���
[Call By]       : main
[Calls]
	printf()	//�޼��� ���
	ThreadUsleep()	//���� �ð����� ���
	pthread_exit()	//thread ����
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
		if (Buf.counter == 0) {	//���� �����Ͱ� ������
			printf("Consumer: Buffer empty. Waiting.....\n");
			while (Buf.counter == 0)//busy waiting
				;
		}

		printf("Consumer: Consuming an item.....\n");
		data = Buf.buf[Buf.out].data;		//dequeue
		Buf.out = (Buf.out + 1) % MAX_BUF;	//out pointer update
		Buf.counter--;				//�о����Ƿ� ������ ���� ����

		ThreadUsleep((rand() % 100) * 10000);	//���� �ð����� ���
	}

	//�����͸� � �о��� ���� queue�� �����Ͱ� �����
	printf("Consumer: Consumed %d items.....\n", i);
	printf("Consumer: %d items in buffer.....\n", Buf.counter);

	//thread ����
	pthread_exit(NULL);
}

/*
[Program Name]	: circular queue multithread program
[Description]	: �� �����尡 shared data�� ���ÿ� ���ٰ����ϸ� busy waiting(CPU����)
[Input]		: Nothing.
[Output]	:
[Calls]
	pthread_create()	//thread ����
	pthread_join()		//thread ���Ḧ ��ٸ�
	srand()			//?
	perror()		//���� �޼��� ���
	exit()			//���μ��� ����
	printf()		//�޼��� ���
*/
int
main()
{
	pthread_t	tid1, tid2;

	srand(0x8888);

	//producer thread ����
	if (pthread_create(&tid1, NULL, (void*)Producer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	//consumer thread ����
	if (pthread_create(&tid2, NULL, (void*)Consumer, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//producer thread�� ���Ḧ ��ٸ�
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}
	//consumer thread�� ���Ḧ ��ٸ�
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	//������ ���� ���
	printf("Main    : %d items in buffer.....\n", Buf.counter);
}
