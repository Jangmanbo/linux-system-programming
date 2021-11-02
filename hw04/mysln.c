#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : create new file(symbolic link) program
[Description]   : argv[1]와 같은 argv[2]파일을 symbolic link로 생성
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 기존 파일과 새 파일 배열
[Output]        : hard link로 만들어진 새 파일
[Calls]
		perror()        //에러 메세지 출력
		fprintf()       //파일에 메세지 출력
		exit()          //프로그램 종료
		link()          //symbolic link로 새 파일 생성
*/

int main(int argc, char* argv[])
{
	if (argc != 3) {//argv!=3이면 에러 메세지 출력 후 프로그램 종료
		fprintf(stderr, "Usage: %s source destinaion\n", argv[0]);
		exit(1);
	}

	//기존 파일인 argv[1]와 동일한 새 파일 argv[2]를 symbolic link로 생성
	if (symlink(argv[1], argv[2]) < 0) {
		//생성에 실패하면 에러 메세지 출력
		perror("link");
		exit(1);
	}
}
