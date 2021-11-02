#include <stdio.h>
#include <unistd.h>
#define	MAX_CMD		256

/*
[Function Name] : DoCmd
[Description]   : 입력받은 command를 수행한 것처럼 출력
[Input]
	char*	cmd	//수행할 command
[Output]        : command를 수행했다는 메세지
[Call By]       : main()
[Calls]
		printf()        //메세지 출력
	sleep()		//입력받은 시간동안 일시정지
[Given]         : Nothing
[Returns]       : Nothing
*/
void
DoCmd(char* cmd)
{
	printf("Doing %s", cmd);
	sleep(1);//1초간 일시정지
	printf("Done\n");
}

/*
[Program Name]  : command program
[Description]   : command를 수행하는 것처럼 출력하는 프로그램
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	printf()	//메세지 출력
	fgets()		//파일 스트림의 버퍼에서 문자열을 읽음
	DoCmd()		//command 실행
*/
int
main()
{
	char	cmd[MAX_CMD];

	while (1) {
		printf("CMD> ");
		//shell에서 command를 읽어옴
		fgets(cmd, MAX_CMD, stdin);
		//command의 첫 문자가 q면 프로그램 종료	
		if (cmd[0] == 'q')
			break;

		//command 실행
		//command 실행이 끝날 때까지 다음 command를 입력받지 못함
		DoCmd(cmd);
	}
}
