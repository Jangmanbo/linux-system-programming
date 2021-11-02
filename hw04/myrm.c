#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : file remove program
[Description]   : argv[1]~argv[argc-1] 파일들 제거
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 제거할 파일들의 배열
[Output]        : Nothing.
[Calls]
		perror()        //에러 메세지 출력
		fprintf()       //파일에 메세지 출력
		exit()          //프로그램 종료
		remove()        //입력으로 받은 파일 제거
*/
int main(int argc, char* argv[])
{
	int		i;

	if (argc == 1) {//제거할 파일이 없다면 프로그램 종료
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}

	for (i = 1; i < argc; i++) {
		if (remove(argv[i]) < 0) {//파일 제거
			//실패하면 에러 메세지 출력
			perror("remove");
			exit(1);
		}
	}
}
