#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

/*
[Program Name]  : directory data display program
[Description]   : 현재 디렉토리의 파일 이름을 출력
[Input]		: Nothing.
[Output]        : 현재 디렉토리 내 파일의 이름
[Calls]
	opendir()	//directory open, directory pointer return
		perror()        //에러 메세지 출력
		printf()       //메세지 출력
		exit()          //프로그램 종료
		readdir()	//디렉토리 데이터 정보 return, 실패하면 return NULL
	closedir()	//디렉토리 검색 종료
*/
int main()
{
	DIR* dp;
	struct dirent* dep;

	//현재 디렉토리의 pointer를 dp structure pointer에 저장
	if ((dp = opendir(".")) == NULL) {
		//실패하면 프로그램 종료
		perror("opendir");
		exit(0);
	}

	//readdir를 실행할 때마다 directory 내의 하나의 file entry를 return
	//모든 파일을 return했으면 NULL을 return
	while (dep = readdir(dp)) {
		//파일 이름 출력
		printf("%s\n", dep->d_name);
	}

	closedir(dp);
}
