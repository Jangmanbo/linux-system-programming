#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static int	Pfd1[2], Pfd2[2];

/*
[Function Name] : TELL_WAIT
[Description]   : 2개의 pipe를 생성하여 초기화
[Input]		: Nothing.
[Output]	: Nothing.
[Call By]       : sync.c의 main()
[Calls]
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
	pipe()		//pipe 생성
[Given]
	static int	Pfd1, Pfd2	//pipe를 생성하기 위한 file descriptor
[Returns]       : Nothing
*/
void
TELL_WAIT(void)
{
	//2개의pipe 생성
	//첫번째 pipe를 제대로 만들지 못하면 다음 pipe를 만들지 않고 종료
	if (pipe(Pfd1) < 0 || pipe(Pfd2) < 0) {
		perror("pipe");
		exit(1);
	}
}


/*
[Function Name] : TELL_PARENT
[Description]   : child process가 parent process에게 메세지 전송
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : sync.c의 main()
[Calls]
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
		write()         //file descriptor로 file에 데이터 write
[Given]
		static int      Pfd2      //메세지를 보내기 위한 file descriptor
[Returns]       : Nothing
*/
void
TELL_PARENT(void)
{
	//Pfd2 file descriptor로 c를 file에 작성
	if (write(Pfd2[1], "c", 1) != 1) {
		perror("write");
		exit(1);
	}
}

/*
[Function Name] : WAIT_PARNET
[Description]   : child process가 parent process로부터의 메세지를 기다림
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : sync.c의 main()
[Calls]
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
		read()          //file에서 데이터 read
	fprintf()	//file에 메세지 출력
[Given]
		static int      Pfd1      //메세지를 읽기 위한 file descriptor
[Returns]       : Nothing
*/
void
WAIT_PARENT(void)
{
	char	c;

	if (read(Pfd1[0], &c, 1) != 1) {
		perror("read");
		exit(1);
	}
	if (c != 'p') {
		fprintf(stderr, "WAIT_PARENT: incorrect data");
		exit(1);
	}
}

/*
[Function Name] : TELL_CHILD
[Description]   : parent process가 child process에게 메세지 전송
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : sync.c의 main()
[Calls]
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
		write()         //file descriptor로 file에 데이터 write
[Given]
		static int      Pfd1      //메세지를 보내기 위한 file descriptor
[Returns]       : Nothing
*/
void
TELL_CHILD(void)
{
	//Pfd1 file descriptor로 p를 file에 작성
	if (write(Pfd1[1], "p", 1) != 1) {
		perror("write");
		exit(1);
	}
}


/*
[Function Name] : WAIT_CHILD
[Description]   : parent process가 child process로부터의 메세지를 기다림
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : sync.c의 main()
[Calls]
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
	read()		//file에서 데이터 read
	fprintf()	//file에 메세지 출력
[Given]
		static int      Pfd2      //메세지를 읽기 위한 file descriptor
[Returns]       : Nothing
*/
void
WAIT_CHILD(void)
{
	char	c;

	//Pfd2로 file에서 char를 읽어옴
	if (read(Pfd2[0], &c, 1) != 1) {
		perror("read");
		exit(1);
	}
	//child process는 항상 데이터로 c를 작성
	//read data가 c가 아니면 incorrect data->프로세스 종료
	if (c != 'c') {
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}
