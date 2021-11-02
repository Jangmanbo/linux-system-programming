#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : remove directory program
[Description]   : argv[1] directory 제거
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 제거할 directory
[Output]        : Nothing.
[Calls]
		perror()        //에러 메세지 출력
		fprintf()       //파일에 메세지 출력
		exit()          //프로그램 종료
		rmdir()        //입력으로 받은 디렉토리 제거
*/
int main(int argc, char* argv[])
{
	if (argc != 2) {//argv!=2면 프로그램 종료
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	//argv[1] directory 제거
	if (rmdir(argv[1]) < 0) {
		//directory 제거에 실패하면 에러 메세지 출력
		perror("rmdir");
		exit(1);
	}
}
