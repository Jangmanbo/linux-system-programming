#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define	MAX_BUF	256

/*
[Program Name]  : print working directory program
[Description]   : 현재 디렉토리의 경로 출력
[Input]
		int argc        // argv 배열의 크기
		char *argv[]
[Output]        : Nothing.
[Calls]
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		exit()          //프로그램 종료
		getcwd()        //현재 작업 디렉토리의 full path를 buf에 저장

*/
int
main(int argc, char* argv[])
{
	char	buf[MAX_BUF];

	//현재 작업 디렉토리의 full path를 buf에 저장
	if (getcwd(buf, MAX_BUF) == NULL) {
		//실패하면 에러 메세지 출력
		perror("getcwd");
		exit(1);
	}

	//현재 작업 디렉토리의 full path 출력
	printf("%s\n", buf);
}
