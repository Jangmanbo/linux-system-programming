#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

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
	pthread_exit()	//thread 종료
[Given]         : Nothing
[Returns]       : Nothing
*/
void
DoCmd(char* cmd)
{
	printf("Doing %s", cmd);
	sleep(1);//1초간 일시정지
	printf("Done\n");

	pthread_exit(NULL);	//thread 종료
}

/*
[Program Name]  : command program
[Description]   : thread를 생성하여 command를 수행하는 것처럼 출력하는 프로그램
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	perror()        //에러 메세지 출력
	printf()	//메세지 출력
		exit()          //프로그램 종료
	fgets()		//파일 스트림의 버퍼에서 문자열을 읽음
	DoCmd()		//command 실행
	pthread_create()//thread 생성
		pthread_join()  //thread가 종료되기를 기다림
*/
int
main()
{
	char		cmd[MAX_CMD];
	pthread_t	tid;

	while (1) {
		printf("CMD> ");

		//shell에서 command를 읽어옴
		fgets(cmd, MAX_CMD, stdin);

		//command의 첫 문자가 q면 프로그램 종료
		if (cmd[0] == 'q')
			break;

		//&tid : 생성한 thread의id
			//NULL : thread의 attribute
			//DoCmd : thread에서 실행할 함수
			//cmd : DoCmd 함수의 파라미터
			//DoCmd(cmd)를 실행하는 thread 생성
		//->현재 thread에서는 command 실행과 무관하게 다음 command를 입력받을 수 있음
		if (pthread_create(&tid, NULL, (void*)DoCmd, (void*)cmd) < 0) {//thread 생성에 실패하면 프로그램 종료
			perror("pthread_create");
			exit(1);
		}

#if 0
		//id가 tid인 thread가 종료될 떄까지 기다림
		pthread_join(tid, NULL);
#endif
	}
}
