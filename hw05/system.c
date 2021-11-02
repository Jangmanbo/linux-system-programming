#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : system program
[Description]   : system이라는 system call을 호출하여 command 실행(child process로 command를 실행하고 child가 끝날 때까지 기다렸다가 끝나면 다음 command 실행)
[Input]		: Nothing.
[Output]        : command 실행 결과
[Calls]
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
	system()	//파라미터로 받은 명령어를 실행, 명령어의 return값을 return, 실패하면 -1 return
	printf		//메세지 출력
*/
int
main()
{
	int		status;

	//status에 date command의 return값이 저장
	if ((status = system("date")) < 0) {//실패하면 프로그램 종료
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//nosuchcommand라는 command는 존재하지 않으므로 status에 -1이 저장, 프로그램 종료
	if ((status = system("nosuchcommand")) < 0) {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//status에 who와 exit 44 두 command의 return값이 저장
	if ((status = system("who; exit 44")) < 0) {//실패하면 프로그램 종료
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
}
