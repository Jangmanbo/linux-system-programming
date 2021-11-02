#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

/*
[Program Name]  : make directory program
[Description]   : argv[1] directory 생성
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 새로 생성할 directory
[Output]        : 새로 만든 directory
[Calls]
	perror()        //에러 메세지 출력
		fprintf()       //파일에 메세지 출력
		exit()          //프로그램 종료
	mkdir()		//디렉토리 생성
*/
int main(int argc, char* argv[])
{
	if (argc != 2) {//argv!=2면 프로그램 종료
		fprintf(stderr, "Usage: %s dirname\n", argv[0]);
		exit(1);
	}

	//argv[1] directory를 생성(permission : rwx-r-xr-x)
	if (mkdir(argv[1], 0755) < 0) {
		//directory 생성에 실패하면 에러 메세지 출력
		perror("mkdir");
		exit(1);
	}
}
