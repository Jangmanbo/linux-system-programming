#include <stdio.h>

/*
[Program Name]  : print envionment variable program
[Description]   : 환경변수 출력
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // status를 출력할 파일들의 배열
	char *envp[]	// 환경변수
[Output]        : 환경변수
[Calls]
	printf()	//메세지 출력
*/
int
main(int argc, char* argv[], char* envp[])
{
	int			i;
	char** p;
	extern char** environ;	//extern문을 통해 다른 시스템에 미리 선언된 환경변수를 가져옴

	printf("List command-line arguments\n");
	//입력받은 파라미터 출력
	for (i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

	printf("\n");
	printf("List environment variables from environ variable\n");

	//C start-up library의 전역변수 environ으로 환경변수를 읽어 출력
	//NULL이 환경변수의 끝을 의미
	for (p = environ; *p != NULL; p++) {
		printf("%s\n", *p);
	}

	printf("\n");
	printf("List environment variables from envp variable\n");
	//3번쨰 파라미터인 envp로 환경변수 출력
	//NULL이 환경변수의 끝을 의미
	for (p = envp; *p != NULL; p++) {
		printf("%s\n", *p);
	}
}
