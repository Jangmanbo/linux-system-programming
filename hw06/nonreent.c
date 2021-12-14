#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
[Function Name] : MyAlarmHandler
[Description]   : SIGALRM이 발생되면 호출, 1초후 SIGALRM을 보냄
[Input]         : signo	//수신한 signal
[Output]        : signal handler 실행 메세지 출력
[Call By]	: kernel
[Calls]
		signal()        //signal handler를 등록
		alarm()         //n초 후에 SIGALRM을 보냄
	printf()	//메세지 출력
	getpwnam()	//user의 password 읽기
	perror()	//에러 메세지 출력
	exit()		//프로그램 종료
[Given]		: Nothing.
[Returns]	: Nothing.
*/

void
MyAlarmHandler(int signo)
{
	struct passwd* rootptr;

	//SIGALRM signal handler 등록
	signal(SIGALRM, MyAlarmHandler);
	alarm(1);	//1초 후 SIGALRM signal 보냄

	printf("in signal handler\n");

	//root user의 password 읽기
	//non-reentrant function인 getpwnam을 signal handler에서 호출
	if ((rootptr = getpwnam("root")) == NULL) {
		perror("getpwnam");
		exit(1);
	}

	return;
}

/*
[Program Name]  : non reent function program
[Description]   : non reent function을 호출하는 signal handler를 등록하여 호출시키는 프로그램
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
		signal()        //signal handler를 등록
		alarm()         //n초 후에 SIGALRM을 보냄
	printf()        //메세지 출력
	strcmp()	//문자열 비교
	getpwnam()      //user의 password 읽기
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
*/
int
main()
{
	struct passwd* ptr;

	//SIGALRM signal handler 등록
	signal(SIGALRM, MyAlarmHandler);
	alarm(1);	//1초 후 SIGALRM signal 보냄

	for (; ; ) {
		//cjs user의 password 읽기
		if ((ptr = getpwnam("cjs")) == NULL) {
			perror("getpwnam");
			exit(1);
		}

		//getpwnam("cjs")를 실행하는 중에 SIGALRM으로 인해 getpwnam("root")이 실행
		//getpwnam("root")이 끝나고 다시 getpwnam("cjs")에 돌아가서 계속 실행해도
		//passwd struct를 중간부터 채워서
		//pw_name은 이전에 실행했던 getpwnam("root")으로 인해  root이므로 프로세스를 종료
		if (strcmp(ptr->pw_name, "cjs") != 0) {
			printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);
			exit(0);
		}
	}
}
