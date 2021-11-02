#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//non-reentrant function�� signal handler���� ȣ���ϴ� ��Ȳ

void
MyAlarmHandler(int signo)
{
	struct passwd* rootptr;

	//SIGALRM signal handler ���
	signal(SIGALRM, MyAlarmHandler);
	alarm(1);	//1�� �� SIGALRM signal ����

	printf("in signal handler\n");

	//root user�� password �б�
	if ((rootptr = getpwnam("root")) == NULL) {
		perror("getpwnam");
		exit(1);
	}

	return;
}

int
main()
{
	struct passwd* ptr;

	//SIGALRM signal handler ���
	signal(SIGALRM, MyAlarmHandler);
	alarm(1);	//1�� �� SIGALRM signal ����

	for (; ; ) {
		//cjs user�� password �б�
		if ((ptr = getpwnam("cjs")) == NULL) {
			perror("getpwnam");
			exit(1);
		}

		//getpwnam("cjs")�� �����ϴ� �߿� SIGALRM���� ���� getpwnam("root")�� ����
		//getpwnam("root")�� ������ �ٽ� getpwnam("cjs")�� ���ư��� ��� �����ص�
		//passwd struct�� �߰����� ä����
		//pw_name�� ������ �����ߴ� getpwnam("root")���� ����  root�̹Ƿ� ���μ����� ����
		if (strcmp(ptr->pw_name, "cjs") != 0) {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}
}
