#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "fifo.h"


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
	char		fname[MAX_FIFO_NAME];
	int			fd, sfd, n;
	MsgType		msg;

	//clinet specific fifo �ʿ�
	//fname�� .fifo�� processid�� ���� ���ڿ� ����
	sprintf(fname, ".fifo%d", getpid());

	//fifo file ����
	//0600 : ���� �а� ����
	if (mkfifo(fname, 0600) < 0) {
		perror("mkfifo");
		exit(1);
	}

	//�б�, ��������� file open 
	//server�κ��� ������� file descriptor�� return
	if ((fd = open(fname, O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}
	//�б�, ��������� file open 
	//server���� ��û�� file descriptor�� return
	if ((sfd = open(SERV_FIFO, O_RDWR)) < 0) {
		perror("open");
		exit(1);
	}

	//client specific fifo�� �̸��� msg.returnFifo�� �����Ͽ� ����
	strcpy(msg.returnFifo, fname);
	//msg.data�� ���ڿ� ����
	sprintf(msg.data, "This is a request from %d.", getpid());
	//sfd file descriptor�� ���� server�� �޼��� ����
	write(sfd, (char*)&msg, sizeof(msg));
	printf("Sent a request.....");

	//client specific fifo file descriptor�� ���� server�κ��� �޼����� read
	if ((n = read(fd, (char*)&msg, sizeof(msg))) < 0) {
		perror("read");
		exit(1);
	}

	printf("Received reply: %s\n", msg.data);

	//close file descriptor
	close(fd);
	close(sfd);

	//remove client specific fifo file
	if (remove(fname) < 0) {
		perror("remove");
		exit(1);
	}
}
