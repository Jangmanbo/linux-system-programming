#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#define	MAX_PATH	256

void
JustDoIt(char* path)
{
	DIR* dp;
	struct dirent* dep;
	struct stat		statbuf;
	char			fullpath[MAX_PATH];

	if ((dp = opendir(path)) == NULL) {//디렉토리 오픈에 실패하면 프로그램 종료
		perror("opendir");
		exit(0);
	}

	long int creat = 2147483647;
	char fname[100];

	while (dep = readdir(dp)) {//디렉토리 내 파일 읽기
		//., ..는 제외
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;

		//파일의 stat 읽기에 실패하면
		if (stat(path, &statbuf) < 0) {
			perror("lstat");
			exit(1);
		}
		
		//파일이 지금까지의 변경 시간보다 더 최근이면 갱신
		if (statbuf.st_atime < creat) {
			strcpy(fname, dep->d_name);
			creat = statbuf.st_atime;
		}
	}

	//현재 디렉토리에서 변경된 시간이 가장 최근인 파일 name 출력
	printf("%s\n", fname);

	closedir(dp);
}

int
main(int argc, char* argv[])
{
	if (argc != 1) {
		fprintf(stderr, "Usage: %s old new\n", argv[0]);
		exit(1);
	}
	JustDoIt(".");
}
