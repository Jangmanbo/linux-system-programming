#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>

char	msg[100];
sem_t	EmptySem, FullSem, MutexSem;

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
	semaphore�� ����ȭ ����
		�ð����� �θ� circular queue�� ������ ����
	queue�� �� �������� �ڸ��� �� ������ ��ٸ�
[Input]
	void	*dummy
[Output]	: thread ������� �� ���� �����͸� �����ߴ���, ���� �����Ͱ� �� ������ ���
[Call By]	: main
[Calls]
	printf()        //�޼��� ���
		perror()        //���� �޼��� ���
		ThreadUsleep()  //���� �ð����� ���
		pthread_exit()  //thread ����
		sem_wait()      //semaphore ���� 1 �̻��̸� ����, �ƴϸ� ��ٸ�
		sem_post()      //semaphore ���� 1 ����, �ش� semaphore�� ��ٸ��� �����带 ����
[Given]
		BoundedBufferType       Buf;                            //������ ������ ����
		sem_t                   EmptySem, FullSem, MutexSem;    //semaphore
[Returns]       : Nothing.
*/
void
Parent(void* dummy)
{
	int		i, data;


	//EmptySem semaphore�� ���� 1�̻��̸�(������ ������ ������ ������) 1 ����(������ ����)
			   //�ƴϸ� 1�̻��� �� ������ ��ٸ�
	if (sem_wait(&EmptySem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}
	//MutexSem�� ��ٸ�(�ٸ� �����尡 critical section�� ���������⸦ ��ٸ�)
	if (sem_wait(&MutexSem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("Parent: Tell to child\n");

	//MutexSem�� ���� 1 ����(critical section�� ���Դٴ� �ǹ�)
		//MutexSem�� ��ٸ��� ������ ����
	if (sem_post(&MutexSem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	if (sem_post(&FullSem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	ThreadUsleep(data);

	printf("Parent: Wait for child to tell\n");
	//Full semaphore�� ���� 1�̻��̸�(�������� ������) 1 ����(������ �Һ�)
	//�ƴϸ� 1�̻��� �� ������ ��ٸ�
	if (sem_wait(&FullSem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	//MutexSem�� ��ٸ�(�ٸ� �����尡 critical section�� ���������⸦ ��ٸ�)
	if (sem_wait(&MutexSem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	//MutexSem�� ���� 1 ����(critical section�� ���Դٴ� �ǹ�)
	//MutexSem�� ��ٸ��� ������ ����
	if (sem_post(&MutexSem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	//EmptySem�� ���� 1 ����
	//EmptySem�� ��ٸ��� ������ ����
	if (sem_post(&EmptySem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	pthread_exit(NULL);
}

/*
[Function Name] : Consumer thread
[Description]
	semaphore�� ����ȭ ����
	�ð����� �θ� circuler queue�� �����͸� �о��
	���� �����Ͱ� ������ ���� ������ ��ٸ�
[Input]
		void    *dummy
[Output]        : thread ������� �� ���� �����͸� �а� ���� �����Ͱ� �� ������ ���
[Call By]       : main
[Calls]
	printf()	//�޼��� ���
	perror()	//���� �޼��� ���
	ThreadUsleep()	//���� �ð����� ���
	pthread_exit()	//thread ����
	sem_wait()	//semaphore ���� 1 �̻��̸� ����, �ƴϸ� ��ٸ�
	sem_post()	//semaphore ���� 1 ����, �ش� semaphore�� ��ٸ��� �����带 ����
[Given]
	sem_t			EmptySem, FullSem, MutexSem;	//semaphore
[Returns]       : Nothing.
*/
void
Child(void* dummy)
{
	int		i, data;

	printf("Child: Wait for parent to tell\n");

	//Full semaphore�� ���� 1�̻��̸�(�������� ������) 1 ����(������ �Һ�)
	//�ƴϸ� 1�̻��� �� ������ ��ٸ�
	if (sem_wait(&FullSem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	//MutexSem�� ��ٸ�(�ٸ� �����尡 critical section�� ���������⸦ ��ٸ�)
	if (sem_wait(&MutexSem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	//MutexSem�� ���� 1 ����(critical section�� ���Դٴ� �ǹ�)
	//MutexSem�� ��ٸ��� ������ ����
	if (sem_post(&MutexSem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	//EmptySem�� ���� 1 ����
	//EmptySem�� ��ٸ��� ������ ����
	if (sem_post(&EmptySem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}

	//���� �ð����� ������ ����
	ThreadUsleep((rand() % 100) * 10000);

	//Full semaphore�� ���� 1�̻��̸�(�������� ������) 1 ����(������ �Һ�)
	//�ƴϸ� 1�̻��� �� ������ ��ٸ�
	if (sem_wait(&EmptySem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	//MutexSem�� ��ٸ�(�ٸ� �����尡 critical section�� ���������⸦ ��ٸ�)
	if (sem_wait(&MutexSem) < 0) {
		perror("sem_wait");
		pthread_exit(NULL);
	}

	printf("Child: Tell to parent\n");

	//MutexSem�� ���� 1 ����(critical section�� ���Դٴ� �ǹ�)
	//MutexSem�� ��ٸ��� ������ ����
	if (sem_post(&MutexSem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	//EmptySem�� ���� 1 ����
	//EmptySem�� ��ٸ��� ������ ����
	if (sem_post(&FullSem) < 0) {
		perror("sem_post");
		pthread_exit(NULL);
	}
	pthread_exit(NULL);	//thread ����
}
/*
[Program Name]  : sipc1.c, sipc2.c semaphore program
[Description]   : sipc1.c, sipc2.c�� semaphore�� ���� ����ȭ�ϴ� ��Ƽ�������  ����
[Input]		: Nothing.
[Output]	: Nothing.
[Calls]         :
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	printf()	//�޼��� ���
	srand()		//���� ����
	sem_init()	//semaphore ����
	sem_destroy()	//semaphore ����
	pthread_create()//thread ����
	pthread_join()	//thread�� ���Ḧ ��ٸ�
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

	srand(0x9999);

	//3���� semaphore�� �ʱⰪ ����
	if (sem_init(&EmptySem, 0, 1) < 0) {
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
	if (pthread_create(&tid1, NULL, (void*)Parent, (void*)NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void*)Child, (void*)NULL) < 0) {
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

	//3���� semaphore ����
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
