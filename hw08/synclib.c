#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

static int	Pfd1[2], Pfd2[2];

/*
[Function Name] : TELL_WAIT
[Description]   : 2���� pipe�� �����Ͽ� �ʱ�ȭ
[Input]		: Nothing.
[Output]	: Nothing.
[Call By]       : sync.c�� main()
[Calls]
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
	pipe()		//pipe ����
[Given]
	static int	Pfd1, Pfd2	//pipe�� �����ϱ� ���� file descriptor
[Returns]       : Nothing
*/
void
TELL_WAIT(void)
{
	//2����pipe ����
	//ù��° pipe�� ����� ������ ���ϸ� ���� pipe�� ������ �ʰ� ����
	if (pipe(Pfd1) < 0 || pipe(Pfd2) < 0) {
		perror("pipe");
		exit(1);
	}
}


/*
[Function Name] : TELL_PARENT
[Description]   : child process�� parent process���� �޼��� ����
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : sync.c�� main()
[Calls]
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
		write()         //file descriptor�� file�� ������ write
[Given]
		static int      Pfd2      //�޼����� ������ ���� file descriptor
[Returns]       : Nothing
*/
void
TELL_PARENT(void)
{
	//Pfd2 file descriptor�� c�� file�� �ۼ�
	if (write(Pfd2[1], "c", 1) != 1) {
		perror("write");
		exit(1);
	}
}

/*
[Function Name] : WAIT_PARNET
[Description]   : child process�� parent process�κ����� �޼����� ��ٸ�
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : sync.c�� main()
[Calls]
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
		read()          //file���� ������ read
	fprintf()	//file�� �޼��� ���
[Given]
		static int      Pfd1      //�޼����� �б� ���� file descriptor
[Returns]       : Nothing
*/
void
WAIT_PARENT(void)
{
	char	c;

	if (read(Pfd1[0], &c, 1) != 1) {
		perror("read");
		exit(1);
	}
	if (c != 'p') {
		fprintf(stderr, "WAIT_PARENT: incorrect data");
		exit(1);
	}
}

/*
[Function Name] : TELL_CHILD
[Description]   : parent process�� child process���� �޼��� ����
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : sync.c�� main()
[Calls]
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
		write()         //file descriptor�� file�� ������ write
[Given]
		static int      Pfd1      //�޼����� ������ ���� file descriptor
[Returns]       : Nothing
*/
void
TELL_CHILD(void)
{
	//Pfd1 file descriptor�� p�� file�� �ۼ�
	if (write(Pfd1[1], "p", 1) != 1) {
		perror("write");
		exit(1);
	}
}


/*
[Function Name] : WAIT_CHILD
[Description]   : parent process�� child process�κ����� �޼����� ��ٸ�
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : sync.c�� main()
[Calls]
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
	read()		//file���� ������ read
	fprintf()	//file�� �޼��� ���
[Given]
		static int      Pfd2      //�޼����� �б� ���� file descriptor
[Returns]       : Nothing
*/
void
WAIT_CHILD(void)
{
	char	c;

	//Pfd2�� file���� char�� �о��
	if (read(Pfd2[0], &c, 1) != 1) {
		perror("read");
		exit(1);
	}
	//child process�� �׻� �����ͷ� c�� �ۼ�
	//read data�� c�� �ƴϸ� incorrect data->���μ��� ����
	if (c != 'c') {
		fprintf(stderr, "WAIT_CHILD: incorrect data");
		exit(1);
	}
}
