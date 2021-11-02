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
[Description]   : file mode ���
[Input]
        struct stat	statbuf	: ������ status�� ����� buffer
[Output]	: Nothing.
[Call By]       : main()
[Calls]         : printf
[Given]         : Nothing
[Returns]       : Nothing
*/
void file_mode(struct stat statbuf) {
    //file type macro�� ���� statbuf�� st_mode���� file type���о�� ���
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

    //bitwise ������ ���� file permission ���
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
[Description]   : user name ���
[Input]
        struct stat     statbuf : ������ status�� ����� buffer
[Output]        : Nothing.
[Call By]       : main()
[Calls]         : printf
[Given]         : Nothing
[Returns]       : Nothing
*/
void user_id(struct stat statbuf) {
    struct passwd* pwd;

    //user ������ pwd�� ����
    pwd = getpwuid(statbuf.st_uid);
    printf(" %s", pwd->pw_name);
}

/*
[Function Name] : print group name
[Description]   : user ID ���
[Input]
        struct stat     statbuf : ������ status�� ����� buffer
[Output]        : Nothing.
[Call By]       : main()
[Calls]         : printf
[Given]         : Nothing
[Returns]       : Nothing
*/
void group_id(struct stat statbuf) {
    struct group* g;

    //group ������ pwd�� ����
    g = getgrgid(statbuf.st_gid);
    printf(" %s", g->gr_name);
}

/*
[Function Name] : print time of last access
[Description]   : �ֱ� ���� �ð��� ���
[Input]
        struct stat     statbuf : ������ status�� ����� buffer
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
[Description]   : ls -l commandó�� ���� ���丮 �� �������� ���� ���
[Input]
        int argc        // argv �迭�� ũ��
        char *argv[]    // status�� ����� ���ϵ��� �迭
[Output]        : ���� ���丮 �� ����, ���丮�� ����
[Calls]
    opendir()	//directory open, directory pointer return
    printf()	//�޼��� ���
    perror()	//���� �޼��� ���
    exit()		//���α׷� ����
    readdir()	//���丮 ������ ���� return, �����ϸ� return NULL
    strcmp()	//�� ���ڿ��� ũ�� ��
    lstat()		//file status�� �а� �������θ� return
    file_mode()	//file mode ���
    user_id()	//user name ���
    group_id()	//group name ���
    last_access_time()	//time of last access ���
    closedir()	//���丮 �˻� ����
*/
int
main()
{
    DIR* dp;
    struct dirent* dep;
    struct stat     statbuf;        //file�� status�� ������ buffer
    char* mode;          //file mode�� ����
    int                     i;
    //���� ���丮�� pointer�� dp structure pointer�� ����
    if ((dp = opendir(".")) == NULL) {
        //�����ϸ� ���α׷� ����
        perror("opendir");
        exit(0);
    }

    //readdir�� ������ ������ directory ���� �ϳ��� file entry�� return
    //��� ������ return������ NULL�� return
    while (dep = readdir(dp)) {
        //., ..�� ������� ����
        if (strcmp(".", dep->d_name) == 0 || strcmp("..", dep->d_name) == 0)
            continue;
        //statbuf�� ������ status�� �о��, �����ϸ� ���� �޼��� ���
        if (lstat(dep->d_name, &statbuf) < 0) {
            perror("lstat");
            continue;
        }

        //file mode ���
        file_mode(statbuf);

        //number of links ���
        printf(" %ld", statbuf.st_nlink);

        //������, �׷�� ���
        user_id(statbuf);
        group_id(statbuf);

        //file size ���
        printf(" %5ld", statbuf.st_size);

        //time of last access ���
        last_access_time(statbuf);

        //���� �̸� ���
        printf(" %s\n", dep->d_name);

    }

    closedir(dp);
}
