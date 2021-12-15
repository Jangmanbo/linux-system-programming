#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

/*
[Program Name]  : fifo clinet program
[Description]   : fifo�� server���� ��û�ϰ� ����޴� client  program
[Input]         : Nothing.
[Output]        : �������� ������� �޼��� ���
[Calls]         :
		exit()          //���μ��� ����
		perror()        //���� �޼��� ���
		mkfifo()        //fifo file ����
		printf()        //�޼��� ���
		close()         //file descriptor close
		open()          //file open, return file descriptor
		read()          //file���� �����͸� read
		sprintf()       //�迭�� ���ڿ� ����
		write()         //file�� ������ write
	strcpy()	//���ڿ� ���
	remove()	//file remove
*/
int
main()
{
	char	fname[128];
	int			fd, sfd, n;
	char	data[128];


	//�б�, ��������� file open 
	//server���� ��û�� file descriptor�� return
	if ((sfd = open("./.myfifo", O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	//msg.data�� ���ڿ� ����
	sprintf(data, "Hello");
	//sfd file descriptor�� ���� server�� �޼��� ����
	write(sfd, data, sizeof(data));
	printf("FIFO2: Sent a message: %s\n", data);

	//fifo file descriptor�� ���� server�κ��� �޼����� read
	if ((n = read(sfd, data, sizeof(data))) < 0) {
		perror("read");
		exit(1);
	}

	printf("FIFO2: Received a message: %s\n", data);

	//close file descriptor
	close(sfd);
}
