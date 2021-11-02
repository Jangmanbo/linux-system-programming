#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : file move program
[Description]   : argv[1]을 argv[2]로 이름 변경을 통해 디렉토리 이동
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 이름을 바꿀 파일과 새로 바꿀 이름의 파일
[Output]        : Nothing.
[Calls]
		perror()        //에러 메세지 출력
		fprintf()       //파일에 메세지 출력
		exit()          //프로그램 종료
		rename()        //파일 이름 변경

*/
int main(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}

	//argv[1]을 argv[2]로 이름 변경
	//파일끼리, 디렉토리끼리 변경 가능(파일-> 디렉토리, 디렉토리 -> 파일 불가능)
	if (rename(argv[1], argv[2]) < 0) {
		//실패하면 에러 메세지 출력
		perror("rename");
		exit(1);
	}
}
