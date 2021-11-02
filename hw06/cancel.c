#include <stdio.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

/*
thread1, thread2가 번갈아서 time quantum이 다 할 때까지 돌다가
Ctrl+C로 SIGINT signal을 전달하면 스레드 종료
*/

pthread_t	ThreadId[2];

void
SigIntHandler(int signo)
{
	int		i;

	printf("Received a SIGINT signal by thread %ld\n", pthread_self());
	printf("Terminate other threads: tid=%ld, %ld\n", ThreadId[0], ThreadId[1]);

	for (i = 0; i < 2; i++) {
		if (pthread_cancel(ThreadId[i])) {
			perror("pthread_cancel");
			exit(1);
		}
	}
	for (i = 0; i < 2; i++) {
		if (pthread_join(ThreadId[i], NULL)) {
			perror("pthread_join");
			exit(1);
		}
	}
	printf("Threads terminated: tid=%ld, %ld\n", ThreadId[0], ThreadId[1]);

	exit(0);
}

void
Thread1(void* dummy)
{
	printf("Thread %ld created.....\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}

void
Thread2(void* dummy)
{
	printf("Thread %ld created.....\n", pthread_self());

	if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
		perror("pthread_setcancelstate");
		pthread_exit(NULL);
	}
	if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
		perror("pthread_setcanceltype");
		pthread_exit(NULL);
	}

	while (1)
		;
}

int
main()
{
	if (pthread_create(&ThreadId[0], NULL, (void*)Thread1, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&ThreadId[1], NULL, (void*)Thread2, NULL) < 0) {
		perror("pthread_create");
		exit(1);
	}

	signal(SIGINT, SigIntHandler);

	printf("Press ^C to quit\n");

	for (; ; )
		pause();
}
