#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

/*
[Function Name] : myexit1
[Description]   : print message
[Input]		: Nothing.
[Output]        : Nothing.
[Call By]       : main()
[Calls]
	printf()	//메세지 출력
[Given]         : Nothing
[Returns]       : Nothing
*/
void
myexit1()
{
	printf("first exit handler\n");
}

/*
[Function Name] : myexit2
[Description]   : print message
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : main()
[Calls]
		printf()        //메세지 출력
[Given]         : Nothing
[Returns]       : Nothing
*/
void
myexit2()
{
	printf("second exit handler\n");
}

/*
[Program Name]  : atexit program
[Description]   : ateexit system call을 호출하여 exit handler를 등록
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
		perror()        //에러 메세지 출력
	printf()	//메세지 출력
		exit()          //프로그램 종료
	atexit()	//exit handler 등록
*/
int
main()
{
	if (atexit(myexit2) != 0) {//myexit2 함수를 exit handler로 등록, 실패하면 프로그램 종료
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0) {//myexit1 함수를 exit handler로 등록, 실패하면 프로그램 종료
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0) {//myexit1 함수를 exit handler로 등록, 실패하면 프로그램 종료
		perror("atexit");
		exit(1);
	}

	//main함수가 종료되었음을 출력
	printf("main is done\n");
	//exit handler 실행
}
