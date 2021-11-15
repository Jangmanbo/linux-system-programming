#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : number square program
[Description]   : 입력받은 숫자의 제곱을 출력
[Input]         :
	int	argc	//입력받은 배열의 크기
	char	*argv[]	//제곱할 숫자
[Output]        : 입력받은 숫자의 제곱
[Calls]
	fprintf()	//파일에 메세지 출력
	exit()		//프로세스 종료
	atoi()		//ASCII to int
	printf()	//메세지 출력
*/
int
main(int argc, char* argv[]) {
	if (argc != 2) {//argv!=3이면 프로그램 종료
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	//char->int 변환
	int num = atoi(argv[1]);

	//제곱 출력
	printf("%d\n", num * num);
}
