#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Function Name] : CharAtaTime
[Description]   : 파라미터로 받은 string을 한글자씩 출력
[Input]
	char* 	str	//출력할 문자열
[Output]        : Nothing.
[Call By]       : main()
[Calls]
		setbuf()	//파일의 버퍼 변경
	putc()		//char 출력
[Given]         : Nothing
[Returns]       : Nothing
*/
void
CharAtaTime(char* str)
{
	char* ptr;
	int		c, i;

	//None buffer, 버퍼를 사용하지 않고 바로바로 출력
	setbuf(stdout, NULL);
	for (ptr = str; c = *ptr++; ) {
		//char 하나를 출력하는데에 시간을 늘림
		//-> time quantum이 다 가서 다른 process가 실행되도록 함
		//-> parent, child process가 번갈아가며 하나의 char를 출력
		for (i = 0; i < 999999; i++)
			;
		putc(c, stdout);//char 출력
	}
}

/*
[Program Name]  : fork program
[Description]   : fork로 child process를 생성
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process 생성
		perror()        //에러 메세지 출력
	exit()          //프로그램 종료
	CharAtaTime()	//파라미터로 받은 string을 한 char씩 출력
*/
int
main()
{
	pid_t	pid;

	//child process 생성
	//parent process라면 pid에 child process의 PID 저장
	//child process라면 pid에 0 저장
	if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종료
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process의 경우
		CharAtaTime("output from child\n");
	}
	else {//parent process의 경우
		CharAtaTime("output from parent\n");
	}
}
