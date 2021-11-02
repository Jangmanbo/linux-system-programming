#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/*
[Function Name] : PirntMsg
[Description]   : print message
[Input]
	char*	msg	//����� �޼��� �迭
[Output]        : Nothing.
[Call By]       : main()
[Calls]
		printf()        //�޼��� ���
	pthread_exit()	//thread ����
[Given]         : Nothing
[Returns]       : Nothing
*/
void
PrintMsg(char* msg)
{
	printf("%s", msg);

	pthread_exit(NULL);//thread ����
}

/*
[Program Name]  : thread program
[Description]   : thread�� ����� �Լ��� ����
[Input]         : Nothing.
[Output]        : PrintMsg�� ���� ���
[Calls]as
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
		printf()        //�޼��� ���
	pthread_create()//thread ����
	pthread_join()	//thread�� ����Ǳ⸦ ��ٸ�
	PrintMsg()	//�޼��� ��� �Լ�
*/
int
main()
{
	pthread_t	tid1, tid2;
	char* msg1 = "Hello, ";
	char* msg2 = "World!\n";

	//&tid : ������ thread��id
	//NULL : thread�� attribute
	//PrintMsg : thread���� ������ �Լ�
	//msg1 : PrintMsg �Լ��� �Ķ����
	//PrintMsg(msg1)�� �����ϴ� thread ����
	if (pthread_create(&tid1, NULL, (void*)PrintMsg, (void*)msg1) < 0) {//thread ������ �����ϸ� ���α׷� ����
		perror("pthread_create");
		exit(1);
	}

	//PrintMsg(msg2)�� �����ϴ� thread ����
	if (pthread_create(&tid2, NULL, (void*)PrintMsg, (void*)msg2) < 0) {//thread ������ �����ϸ� ���α׷� ����
		perror("pthread_create");
		exit(1);
	}

	//�� thread�� ���������� ���
	printf("Threads created: tid=%ld, %ld\n", tid1, tid2);

	//tid1�� id�� thread�� ����Ǳ⸦ ��ٸ�
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	//tid2�� id�� thread�� ����Ǳ⸦ ��ٸ�
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	//�� thread�� ����Ǿ����� ���
	printf("Threads terminated: tid=%ld, %ld\n", tid1, tid2);
}
