#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

/*
[Function Name] : print file mode
[Description]   : file mode 출력
[Input]
        struct stat	statbuf	: 파일의 status가 저장된 buffer
[Output]	: Nothing.
[Call By]       : main()
[Calls]         : printf
[Given]         : Nothing
[Returns]       : Nothing
*/
void file_mode(struct stat statbuf) {
    //file type macro를 통해 statbuf의 st_mode에서 file type을읽어와 출력
    if (S_ISREG(statbuf.st_mode))
        printf("%s", "-");
    else if (S_ISDIR(statbuf.st_mode))
        printf("%s", "d");
    else if (S_ISCHR(statbuf.st_mode))
        printf("%s", "c");
    else if (S_ISBLK(statbuf.st_mode))
        printf("%s", "b");
    else if (S_ISLNK(statbuf.st_mode))
        printf("%s", "l");

    //bitwise 연산을 통해 file permission 출력
    if (statbuf.st_mode & S_IRUSR)
        printf("%s", "r");
    else
        printf("%s", "-");
    if (statbuf.st_mode & S_IWUSR)
        printf("%s", "w");
    else
        printf("%s", "-");
    if (statbuf.st_mode & S_IXUSR)
        printf("%s", "x");
    else
        printf("%s", "-");
    if (statbuf.st_mode & S_IRGRP)
        printf("%s", "r");
    else
        printf("%s", "-");
    if (statbuf.st_mode & S_IWGRP)
        printf("%s", "w");
    else
        printf("%s", "-");
    if (statbuf.st_mode & S_IXGRP)
        printf("%s", "x");
    else
        printf("%s", "-");
    if (statbuf.st_mode & S_IROTH)
        printf("%s", "r");
    else
        printf("%s", "-");
    if (statbuf.st_mode & S_IWOTH)
        printf("%s", "w");
    else
        printf("%s", "-");
    if (statbuf.st_mode & S_IXOTH)
        printf("%s", "x");
    else
        printf("%s", "-");
}

/*
[Function Name] : print user name
[Description]   : user name 출력
[Input]
        struct stat     statbuf : 파일의 status가 저장된 buffer
[Output]        : Nothing.
[Call By]       : main()
[Calls]         : printf
[Given]         : Nothing
[Returns]       : Nothing
*/
void user_id(struct stat statbuf) {
    struct passwd* pwd;

    //user 정보를 pwd에 저장
    pwd = getpwuid(statbuf.st_uid);
    printf(" %s", pwd->pw_name);
}

/*
[Function Name] : print group name
[Description]   : user ID 출력
[Input]
        struct stat     statbuf : 파일의 status가 저장된 buffer
[Output]        : Nothing.
[Call By]       : main()
[Calls]         : printf
[Given]         : Nothing
[Returns]       : Nothing
*/
void group_id(struct stat statbuf) {
    struct group* g;

    //group 정보를 pwd에 저장
    g = getgrgid(statbuf.st_gid);
    printf(" %s", g->gr_name);
}

/*
[Function Name] : print time of last access
[Description]   : 최근 접근 시간을 출력
[Input]
        struct stat     statbuf : 파일의 status가 저장된 buffer
[Output]        : Nothing.
[Call By]       : main()
[Calls]         : printf
[Given]         : Nothing
[Returns]       : Nothing
*/
void last_access_time(struct stat statbuf) {
    time_t	atime = statbuf.st_atime;
    char	buf[40];

    strftime(buf, 40, "%b %d %H:%M", localtime(&atime));
    printf(" %s", buf);
}

/*
[Program Name]  : my ls-l command program
[Description]   : ls -l command처럼 현재 디렉토리 내 데이터의 정보 출력
[Input]
        int argc        // argv 배열의 크기
        char *argv[]    // status를 출력할 파일들의 배열
[Output]        : 현재 디렉토리 내 파일, 디렉토리의 정보
[Calls]
    opendir()	//directory open, directory pointer return
    printf()	//메세지 출력
    perror()	//에러 메세지 출력
    exit()		//프로그램 종료
    readdir()	//디렉토리 데이터 정보 return, 실패하면 return NULL
    strcmp()	//두 문자열의 크기 비교
    lstat()		//file status를 읽고 성공여부를 return
    file_mode()	//file mode 출력
    user_id()	//user name 출력
    group_id()	//group name 출력
    last_access_time()	//time of last access 출력
    closedir()	//디렉토리 검색 종료
*/
int
main()
{
    DIR* dp;
    struct dirent* dep;
    struct stat     statbuf;        //file의 status를 저장할 buffer
    char* mode;          //file mode을 저장
    int                     i;
    //현재 디렉토리의 pointer를 dp structure pointer에 저장
    if ((dp = opendir(".")) == NULL) {
        //실패하면 프로그램 종료
        perror("opendir");
        exit(0);
    }

    //readdir를 실행할 때마다 directory 내의 하나의 file entry를 return
    //모든 파일을 return했으면 NULL을 return
    while (dep = readdir(dp)) {
        //., ..는 출력하지 않음
        if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
            continue;
        //statbuf에 파일의 status를 읽어옴, 실패하면 에러 메세지 출력
        if (lstat(dep->d_name, &statbuf) < 0) {
            perror("lstat");
            continue;
        }

        //file mode 출력
        file_mode(statbuf);

        //number of links 출력
        printf(" %ld", statbuf.st_nlink);

        //유저명, 그룹명 출력
        user_id(statbuf);
        group_id(statbuf);

        //file size 출력
        printf(" %5ld", statbuf.st_size);

        //time of last access 출력
        last_access_time(statbuf);

        //파일 이름 출력
        printf(" %s\n", dep->d_name);

    }

    closedir(dp);
}
