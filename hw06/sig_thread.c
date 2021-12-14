#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define	THREAD_MAIN

/*
#define	THREAD_1
#define	THREAD_2
*/



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
	exit()		//���μ��� ����
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal by thread %ld\n", pthread_self());
	printf("Terminate this process\n");

	exit(0);
}

/*
[Function Name] : Thread1
[Description]   : SIGINT signal handler�� ���
[Input]         : dummy
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
	signal()	//signal handler�� ���
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
Thread1(void* dummy)
{
#ifdef	THREAD_1
	signal(SIGINT, SigIntHandler);
#endif

	while (1)
		;
}

/*
[Function Name] : Thread2
[Description]   : SIGINT signal handler�� ���
[Input]         : dummy
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
		signal()        //signal handler�� ���
[Given]         : Nothing.
[Return]        : Nothing.
*/
void
Thread2(void* dummy)
{
#ifdef	THREAD_2
	signal(SIGINT, SigIntHandler);
#endif

	while (1)
		;
}

/*
[Program Name]  : main thread signal handler program
[Description]   : � thread���� signal handler�� ����ص� main thread����signal ó������ ���̴� ���α׷�
[Input]		: Nothing.
[Output]        : main thread�� ���� ������ thread�� tid ���
[Calls]
	pthread_create()//thread ����
		signal()        //signal handler�� ���
	printf()        //�޼��� ���
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
	pause()		//signal�� �߻��� ������ ���
*/
int
main()
{

	pthread_t	tid1, tid2;

	//&tid1 : ������ thread��id
	//NULL : thread�� attribute
	//Thread1 : thread���� ������ �Լ�
	//NULL : Thread1�Լ��� �Ķ���� ����
	//Thread1()�� �����ϴ� thread ����
	if (pthread_create(&tid1, NULL, (void*)Thread1, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//&tid2 : ������ thread��id
		//NULL : thread�� attribute
		//Thread2 : thread���� ������ �Լ�
		//NULL : Thread2�Լ��� �Ķ���� ����
		//Thread2()�� �����ϴ� thread ����
	if (pthread_create(&tid2, NULL, (void*)Thread2, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	//main thread�� ���� ������ thread�� tid ���
	printf("Create two threads: tid1=%ld, tid2=%ld\n", tid1, tid2);
	printf("Main thread: tid=%ld\n", pthread_self());

#ifdef	THREAD_MAIN
	signal(SIGINT, SigIntHandler);
#endif

	printf("Press ^C to quit\n");

	for (; ; )
		pause();	//signal�� �߻��� ������ ���
}
