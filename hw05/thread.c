#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

/*
[Function Name] : PirntMsg
[Description]   : print message
[Input]
	char*	msg	//출력할 메세지 배열
[Output]        : Nothing.
[Call By]       : main()
[Calls]
		printf()        //메세지 출력
	pthread_exit()	//thread 종료
[Given]         : Nothing
[Returns]       : Nothing
*/
void
PrintMsg(char* msg)
{
	printf("%s", msg);

	pthread_exit(NULL);//thread 종료
}

/*
[Program Name]  : thread program
[Description]   : thread를 만들어 함수를 실행
[Input]         : Nothing.
[Output]        : PrintMsg의 실행 결과
[Calls]as
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
		printf()        //메세지 출력
	pthread_create()//thread 생성
	pthread_join()	//thread가 종료되기를 기다림
	PrintMsg()	//메세지 출력 함수
*/
int
main()
{
	pthread_t	tid1, tid2;
	char* msg1 = "Hello, ";
	char* msg2 = "World!\n";

	//&tid : 생성한 thread의id
	//NULL : thread의 attribute
	//PrintMsg : thread에서 실행할 함수
	//msg1 : PrintMsg 함수의 파라미터
	//PrintMsg(msg1)를 실행하는 thread 생성
	if (pthread_create(&tid1, NULL, (void*)PrintMsg, (void*)msg1) < 0) {//thread 생성에 실패하면 프로그램 종료
		perror("pthread_create");
		exit(1);
	}

	//PrintMsg(msg2)를 실행하는 thread 생성
	if (pthread_create(&tid2, NULL, (void*)PrintMsg, (void*)msg2) < 0) {//thread 생성에 실패하면 프로그램 종료
		perror("pthread_create");
		exit(1);
	}

	//두 thread를 생성했음을 출력
	printf("Threads created: tid=%ld, %ld\n", tid1, tid2);

	//tid1이 id인 thread가 종료되기를 기다림
	if (pthread_join(tid1, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	//tid2가 id인 thread가 종료되기를 기다림
	if (pthread_join(tid2, NULL) < 0) {
		perror("pthread_join");
		exit(1);
	}

	//두 thread를 종료되었음을 출력
	printf("Threads terminated: tid=%ld, %ld\n", tid1, tid2);
}
