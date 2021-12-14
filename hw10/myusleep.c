#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

/*
[Function Name] : myusleep
[Description]   : us 단위로 sleep
[Input]
	int	secs	//s 단위
	int	usecs	//us 단위
[Output]        : Nothing.
[Call By]       : main
[Calls]
		printf()        //메세지 출력
	select()	//여러 개의 socket을 처리(이 함수에서는 단순히 sleep하는 용도)
[Given]		: Nothing.
[Returns]       : Nothing.
*/
void
myusleep(int secs, int usecs)
{
	struct timeval	tm;	//상대 시간(현재부터 얼만큼 자는지)

	tm.tv_sec = secs;	//s 단위
	tm.tv_usec = usecs;	//us 단위

	//tm에 저장된 시간만큼 sleep
	if (select(0, (fd_set*)0, (fd_set*)0, (fd_set*)0, &tm) < 0) {
		perror("select");
		return;
	}
}

/*
[Program Name]  : myusleep program
[Description]   : 1.5초간 sleep 후 종료
[Input]         : Nothing.
[Output]        : 몇초간 자는지 출력
[Calls]         :
		printf()        //메세지 출력
	myusleep()	//원하는 시간동안 sleep
*/
int
main()
{
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);
}
