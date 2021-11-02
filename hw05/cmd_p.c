#include <stdio.h>
#include <stdlib.h>
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
[Description]   : child process를 생성하여 command를 수행하는 것처럼 출력하는 프로그램
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()          //child process 생성
	perror()        //에러 메세지 출력
	printf()	//메세지 출력
		exit()          //프로그램 종료
	fgets()		//파일 스트림의 버퍼에서 문자열을 읽음
	DoCmd()		//command 실행
*/
int
main()
{
	char	cmd[MAX_CMD];
	int		pid;

	while (1) {
		printf("CMD> ");
		//shell에서 command를 읽어옴
		fgets(cmd, MAX_CMD, stdin);
		//command의 첫 문자가 q면 프로그램 종료
		if (cmd[0] == 'q')
			break;

		//child process 생성
		//parent process라면 pid에 child process의 PID 저장
		//child process라면 pid에 0 저장
		if ((pid = fork()) < 0) {//fork의 return value가 음수이면 error, 프로그램 종료
			perror("fork");
			exit(1);
		}

		//child process가 command를 실행
				//-> parent process는 command 실행과 무관하게 다음 command를 입력받을 수 있음
		else if (pid == 0) {
			DoCmd(cmd);//command 실행
			exit(0);//process 종료
		}
#if 0
		else {//parent process의 경우
			//child process의 종료를 기다림
			wait(NULL);
		}
#endif
	}
}
