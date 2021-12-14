#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "prodcons.h"

char	pData[100];
int 	count = 0;
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
	�����͸� ����
		mutex, condition variable�� ����Ͽ� synchronization ����
[Input]
	void	*dummy
[Output]	: Nothing.
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
	phtread_cond_t  NotFull, NotEmpty, Mutex;
[Returns]       : Nothing.
*/
void
Producer(void* dummy)
{
	int		i, data;

	printf("Producer: Start.....\n");

	//mutex lock
	if (pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	//�����Ͱ� �̹� ������
	while (count == 1) {
		//NotFull conditino variable�� ��ٸ�
		if (pthread_cond_wait(&NotFull, &Mutex) < 0) {
			perror("pthread_cond_wait");
			pthread_exit(NULL);
		}
	}
	
	sprintf(pData, "This is a request from %ld.", pthread_self());
	printf("Sent a request.....\n");

	count++;	//�����͸� ���������Ƿ� ������ ���� ����

	//NotEmpty condition variable��pthread_cond_wait���� ��ٸ��� �����带 ����
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

	printf("Received reply : %s\n", pData);

	pthread_exit(NULL);	//thread ����
}

/*
[Function Name] : Consumer thread
[Description]
	�����͸� �о��
	mutex, condition variable�� ����Ͽ� synchronization ����
[Input]
		void    *dummy
[Output]        : Nothing.
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
	phtread_cond_t	NotFull, NotEmpty, Mutex;
[Returns]       : Nothing.
*/
void
Consumer(void* dummy)
{
	int		i, data;

	printf("Consumer: Start.....\n");

	//mutex lock
	if (pthread_mutex_lock(&Mutex) < 0) {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}

	//���� �����Ͱ� ������
	while (count == 0) {
		//NotEmpty condition varaible�� ��ٸ�
		if (pthread_cond_wait(&NotEmpty, &Mutex) < 0) {
			perror("pthread_cond_wait");
			pthread_exit(NULL);
		}
	}

	printf("Received request: %s.....\n", pData);
	sprintf(pData, "This is a reply from %ld.", pthread_self());

	//�о����Ƿ� ������ ����
	count--;

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

	pthread_exit(NULL);	//thread ����
}

/*
[Program Name]	: sipc1.c, sipc2.c mutex&condition variable program
[Description]	: sipc1.c, sipc2.c�� mutex, condition variable�� �̿��� ����ȭ ��Ƽ������ ���α׷����� ����
[Input]		: Nothing.
[Output]	: Nothing.
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
/*
	semaphore�� mutex&condition variable�� ����
	semaphore : ���� ������ ���� ���ҽ�Ű�� ��ٸ��� �ִ� �����尡 signal�� ����
	mutex&condition variable�� wait�� �ص� signal�� ���� ����
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
