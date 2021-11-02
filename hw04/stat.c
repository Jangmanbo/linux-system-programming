#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
[Program Name]  : status print program
[Description]   : argv[1]~argv[argc-1] 파일들의 status를 출력
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // status를 출력할 파일들의 배열
[Output]        : 파일들의 status
[Calls]
		perror()        //에러 메세지 출력
		printf()	//메세지 출력
	lstat()         //file status를 읽고 성공여부를 return
*/

int main(int argc, char* argv[])
{
	struct stat	statbuf;	//file의 status를 저장할 buffer
	char* mode;		//file type을 저장
	int			i;

	//입력으로 받은 파일들의 status를 출력
	for (i = 1; i < argc; i++) {
		printf("%s: ", argv[i]);
		//statbuf에 status를 읽어옴, 실패하면 에러 메세지 출력
		if (lstat(argv[i], &statbuf) < 0) {
			perror("lstat");
			continue;
		}

		//file type macro를 통해 statbuf의 st_mode에서 file type을읽어와 출력
		if (S_ISREG(statbuf.st_mode))
			mode = "regular";
		else if (S_ISDIR(statbuf.st_mode))
			mode = "directory";
		else if (S_ISCHR(statbuf.st_mode))
			mode = "character special";
		else if (S_ISBLK(statbuf.st_mode))
			mode = "block special";
		else if (S_ISFIFO(statbuf.st_mode))
			mode = "FIFO";
		else if (S_ISLNK(statbuf.st_mode))
			mode = "symbolic link";
		else if (S_ISSOCK(statbuf.st_mode))
			mode = "socket";

		//file status 출력
		printf("%s\n", mode);
		printf("\tst_mode = %u\n", statbuf.st_mode);
		printf("\tst_ino = %lu\n", statbuf.st_ino);
		printf("\tst_dev = %lu\n", statbuf.st_dev);
		printf("\tst_rdev = %lu\n", statbuf.st_rdev);
		printf("\tst_nlink = %lu\n", statbuf.st_nlink);
		printf("\tst_uid = %d\n", statbuf.st_uid);
		printf("\tst_gid = %d\n", statbuf.st_gid);
		printf("\tst_size = %ld\n", statbuf.st_size);
		printf("\tst_atime = %ld\n", statbuf.st_atime);
		printf("\tst_mtime = %ld\n", statbuf.st_mtime);
		printf("\tst_ctime = %ld\n", statbuf.st_ctime);
		printf("\tst_blksize = %ld\n", statbuf.st_blksize);
		printf("\tst_blocks = %ld\n", statbuf.st_blocks);
	}
}
