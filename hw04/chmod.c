#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

/*
[Program Name]  : file mode change program
[Description]   : file mode를 변경
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
		perror()        //에러 메세지 출력
		exit()          //프로그램 종료
	stat()		//file status를 buffer에 저장, 성공여부를 return
	chmod()		//file mode 변경, 성공여부를 return
*/
int main()
{
	struct stat	statbuf;	//file status를 저장할 buffer

	//bar file의 status를 statbuf에 저장
	if (stat("bar", &statbuf) < 0) {
		//실패하면 에러 메세지 출력
		perror("stat");
		exit(1);
	}

	//bar file의 mode를 변경
	//& ~S_IWGRP : 현재 file mode에서 S_IWGRP 비트를 0으로 변경
	//| S_ISUID : 현재 file mode에서 S_ISUID 비트를 1로 변경
	if (chmod("bar", (statbuf.st_mode & ~S_IWGRP) | S_ISUID) < 0) {
		//실패하면 에러 메세지 출력
		perror("chmod");
		exit(1);
	}

	//foo file의 mode를 S_IRUSR, S_IWUSR, S_IRGRP, S_IROTH 비트만 1, 나머지 비트는 0으로 변경
	if (chmod("foo", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0) {
		//실패하면 에러 메세지 출력
		perror("chmod");
		exit(1);
	}
}
