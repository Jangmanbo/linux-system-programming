#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#define	MAX_PATH	256

/*
[Function Name] : JustDoIt
[Description]   : 디렉토리의 데이터 출력
[Input]
		char	*path	 : 데이터를 읽을 디렉토리의 경로
[Output]	: 디렉토리의 데이터
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

	printf("\n%s:\n", path);

	//readdir를 실행할 때마다 directory 내의 하나의 데이터를 return
		//모든 데이터를 return했으면 NULL을 return
	while (dep = readdir(dp)) {
		//., ..은 출력 X
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		//파일 이름 출력
		printf("%s\n", dep->d_name);
	}

	//디렉토리 정보 읽기 위치를 처음으로 이동
	rewinddir(dp);

	//readdir를 실행할 때마다 directory 내의 하나의 데이터를 return
		//모든 데이터를 return했으면 NULL을 return
	while (dep = readdir(dp)) {
		//., ..은 출력 X
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		strcpy(fullpath, path);	//fullpath에 path 문자열 복사
		strcat(fullpath, "/");	//fullpath에 /을 이어붙임
		strcat(fullpath, dep->d_name);	//fullpath에 파일 이름을 이어붙임
		//statbuf에 fullpath에 대한 status를 읽어옴
		if (lstat(fullpath, &statbuf) < 0) {
			perror("lstat");
			exit(1);
		}

		//directory면 재귀 호출
		if (S_ISDIR(statbuf.st_mode)) {
			JustDoIt(fullpath);
		}
	}

	closedir(dp);
}

/*
[Program Name]  : directory data display program
[Description]   : 현재 디렉토리의 파일, 디렉토리 이름을 출력
[Input]         : Nothing.
[Output]        : 현재 디렉토리 내 파일, 디렉토리의 이름
[Calls]
	JustDoIt()	//현재 디렉토리의 데이터를 출력
*/
int
main()
{
	JustDoIt(".");	//현재 디렉토리의 데이터를 출력
}
