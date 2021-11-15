#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#define	MAX_PATH	256

/*
[Function Name] : JustDoIt
[Description]   : 변경된 시간이 가장 최근인 파일 이름 출력
[Input]
		char	*path	 : 데이터를 읽을 디렉토리의 경로
[Output]	: 변경된 시간이 가장 최근인 파일 이름
[Call By]       : main()
[Calls]
		opendir()       //directory open, directory pointer return
		perror()        //에러 메세지 출력
		printf()        //메세지 출력
		exit()          //프로그램 종료
		readdir()       //디렉토리 데이터 정보 return, 실패하면 return NULL
	rewinddir()	//디렉토리 정보 읽기 위치를 처음으로 이동
	lstat()		//file status를 일고 성공여부를 return
	closedir()      //디렉토리 검색 종료
	strcmp()	//두 문자열의 크기 비교
	strcpy()	//문자열 복사
	strcat()	//문자열 이어붙이기
	JustDoIt()	//재귀 호출

[Given]         : Nothing
[Returns]       : Nothing
*/
void
JustDoIt(char* path)
{
	DIR* dp;
	struct dirent* dep;
	struct stat		statbuf;
	char			fullpath[MAX_PATH];

	//현재 디렉토리의 pointer를 dp structure pointer에 저장	
	if ((dp = opendir(path)) == NULL) {
		//실패하면 종료
		perror("opendir");
		exit(0);
	}

	long int modify = 0;	//파일 변경 시간
	char fname[100];	//파일 이름

	//readdir를 실행할 때마다 directory 내의 하나의 데이터를 return
	//모든 데이터를 return했으면 NULL을 return
	while (dep = readdir(dp)) {
		//., ..은 출력 X
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		//statbuf에서 파일에 대한 status를 읽어옴
		if (stat(dep->d_name, &statbuf) < 0) {
			perror("lstat");
			exit(1);
		}
		//지금까지의 파일 변경 시간 보다 최근이면 갱신
		if (statbuf.st_mtime > modify) {
			strcpy(fname, dep->d_name);
			modify = statbuf.st_atime;
		}
	}

	//파일 변경 시간이 가장 최근인 파일 이름 출력
	printf("%s\n", fname);

	closedir(dp);
}

/*
[Program Name]  : last modify file name print program
[Description]   : 변경된 시간이 가장 최근인 파일 이름 출력
[Input]         :
	int	argc	//입력한 배열의 크기
	char	*argv	//프로그램
[Output]        : 변경된 시간이 가장 최근인 파일 이름
[Calls]
	JustDoIt()	//변경된 시간이 가장 최근인 파일 이름을 출력
*/
int
main(int argc, char* argv[])
{
	if (argc != 1) {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}
	JustDoIt(".");	//현재 디렉토리의 데이터를 출력
}
