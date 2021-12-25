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
main(int argc, char* argv[])
{
	char	fname[128];
	int			fd, sfd, n;
	char	data[128] = "";


	//�б�, ��������� file open 
	//server���� ��û�� file descriptor�� return
	if ((sfd = open("./.myfifo", O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	if (argc == 2) {
		char command[128] = "";
		strcat(command, "cat ");
		strcat(command, argv[1]);
		strcat(command, ".txt");
		system(command);

		exit(0);
	}
	for (int i = 1; i < argc; i++) {
		strcat(data, argv[i]);
		strcat(data, " ");
	}

	//msg.data�� ���ڿ� ����
	//sprintf(data, "Hello");
	//sfd file descriptor�� ���� server�� �޼��� ����
	write(sfd, data, sizeof(data));

	//close file descriptor
	close(sfd);
}
