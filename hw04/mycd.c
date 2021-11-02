#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : change working directory program
[Description]   : argv[1] directory로 이동
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 이동할 directory
[Output]        : Nothing.
[Calls]
		perror()        //에러 메세지 출력
	fprintf()	//파일에 메세지 출력
		exit()          //프로그램 종료
		chdir()         //working directory를 입력으로 받은 디렉토리로 변경

*/
int
main(int argc, char* argv[])
{
	if (argc != 2) {//argc!=2면 프로그램 종료
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	//argv[1] directory로 이동(프로그램 내에서 경로를 이동해 작업하는 것이므로 명령 프롬프트에서는 바뀌지 않음)
	if (chdir(argv[1]) < 0) {
		//실패하면 에러메세지 출력
		perror("chdir");
		exit(1);
	}
}
