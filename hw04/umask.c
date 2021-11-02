#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
[Program Name]  : file mode creation mask program
[Description]   : file mode의 특정 비트를 끄고 bar, foo file을 생성
[Input]		: Nothing.
[Output]        : Nothing
[Calls]
		perror()        //에러 메세지 출력
	exit()		//프로그램 종료
	umask()		//file mode에서 입력으로 받은 비트를 끄고 생성하도록 mask
	creat()		//입력으로 받은 이름과 permission의파일을 생성, 성공여부를 return
*/
int main()
{
	umask(0);	//mask를 하나도 하지 않음
	//file mode가 rw-rw-rw인 bar file 생성
	if (creat("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
		S_IROTH | S_IWOTH) < 0) {
		//file 생성에 실패하면 에러 메세지 출력
		perror("creat");
		exit(1);
	}

	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);	//file mode에서 ---rw-rw-에 해당하는 비트를 끄고 생성하도록 mask
	//file mode를 rw-rw-rw-로 하려 하지만 ---rw-rw- 비트를 꺼야 하므로 rw-------인 foo file 생성
	if (creat("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
		S_IROTH | S_IWOTH) < 0) {
		//file 생성에 실패하면 에러 메세지 출력
		perror("creat");
		exit(1);
	}
}
