#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>


pthread_t	ThreadId[2];

/*
[Function Name] : SigIntHandler
[Description]   :
	� thread���� signal handler�� ����ص� main thread����signal ó������ ����
	���μ��� ����
[Input]         :
	signo	//������ signal
[Output]        : signal�� ó���ϴ� thread�� tid ���
[Call By]       : kernel
[Calls]         :
	printf()	//�޼��� ���
	perror()	//���� �޼��� ���
	exit()		//���μ��� ����
	pthread_cancel()//thread �ߴ�
	pthread_join()	//thread�� ���Ḧ ��ٸ�
[Given]         : ThreadId[2]
[Return]        : Nothing.
*/
void
SigIntHandler(int signo)
{
	int		i;

	//���� ������ ID�� ���� ������ �� thread terminate �޼��� ���
	printf("Received a SIGINT signal by thread %ld\n", pthread_self());
	printf("Terminate other threads: tid=%ld, %ld\n", ThreadId[0], ThreadId[1]);

	//�� thread �ߴ�
	for (i = 0; i < 2; i++) {
		if (pthread_cancel(ThreadId[i])) {
			perror("pthread_cancel");
			exit(1);
		}
	}
	//�� thread�� ���Ḧ ��ٸ�
	for (i = 0; i < 2; i++) {
		if (pthread_join(ThreadId[i], NULL)) {
			perror("pthread_join");
			exit(1);
		}
	}
	//�� thread�� ����Ǿ����� �˸�
	printf("Threads terminated: tid=%ld, %ld\n", ThreadId[0], ThreadId[1]);

	exit(0);	//���μ��� ����
}

/*
[Function Name] : Thread1
[Description]   : thread1�� ��� ��û �� ������ ����
[Input]         : dummy
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
	pthread_setcancelstate()	//pthread_cancel�� ���� ��� ��û�� �޾��� �� ��� �������� ����
	pthread_setcanceltype()		//��� ��û�� �޾Ƶ��� �� ��� �������� �������� ����
	pthread_exit()			//thread ����
	perror()			//���� �޼��� ���
	printf()			//�޼��� ���
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
Thread1(void* dummy)
{
	printf("Thread %ld created.....\n", pthread_self());

	//��� ��û�� �޾Ƶ��̱�� ����
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}

	//thread ��� �����ϱ�� ����
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	//time quantum�� �� �� ������ ����
	while (1)
		;
}

/*
[Function Name] : Thread2
[Description]   : thread2�� ��� ��û �� ������ ����
[Input]         : dummy
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
		pthread_setcancelstate()        //pthread_cancel�� ���� ��� ��û�� �޾��� �� ��� �������� ����
		pthread_setcanceltype()         //��� ��û�� �޾Ƶ��� �� ��� �������� �������� ����
		pthread_exit()                  //thread ����
		perror()                        //���� �޼��� ���
		printf()                        //�޼��� ���
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
Thread2(void* dummy)
{
	printf("Thread %ld created.....\n", pthread_self());

	//��� ��û�� �޾Ƶ��̱�� ����
	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	//thread�� ��� �����ϱ�� ����
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	//time quantum�� �� �� ������ ����
	while (1)
		;
}

/*
[Program Name]  : thread terminate program
[Description]   :
	�� ���� thread �����ϰ� ��� ��û �� ���� ����
	thread1, thread2�� �����Ƽ� time quantum�� �� �� ������ ���ٰ�
	Ctrl+C�� SIGINT signal�� �����ϸ� ������ ����
[Input]         : Nothing.
[Output]        : �� ���� thread ����
[Calls]
	pthread_create()//thread ����
	perror()	//���� �޼��� ���
	exit()		//���α׷� ����
	signal()	//signal handler ���
	printf()	//�޼��� ���
	pause()		//signal�� �߻��� ������ ���
*/
int
main()
{
	//&ThreadId[0] : ������ thread��id
		//NULL : thread�� attribute
		//Thread1 : thread���� ������ �Լ�
		//NULL : Thread1�Լ��� �Ķ���� ����
		//Thread1()�� �����ϴ� thread ����
	if (pthread_create(&ThreadId[0], NULL, (void*)Thread1, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//&ThreadIdd2 : ������ thread��id
		//NULL : thread�� attribute
		//Thread2 : thread���� ������ �Լ�
		//NULL : Thread2�Լ��� �Ķ���� ����
		//Thread2()�� �����ϴ� thread ����
	if (pthread_create(&ThreadId[1], NULL, (void*)Thread2, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//SIGINT signal handler ���
	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit\n");

	for (; ; )
		pause();	//signal�� �߻��� ������ ���
}
