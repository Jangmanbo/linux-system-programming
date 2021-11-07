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

	if ((dp = opendir(path)) == NULL) {
		perror("opendir");
		exit(0);
	}

	long int creat = 2147483647;
	char fname[100];

	while (dep = readdir(dp)) {
		if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
			continue;
		if (stat(path, &statbuf) < 0) {
			perror("lstat");
			exit(1);
		}
		if (statbuf.st_atime < creat) {
			strcpy(fname, dep->d_name);
			creat = statbuf.st_atime;
		}
	}

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
