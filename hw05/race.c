#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Function Name] : CharAtaTime
[Description]   : �Ķ���ͷ� ���� string�� �ѱ��ھ� ���
[Input]
	char* 	str	//����� ���ڿ�
[Output]        : Nothing.
[Call By]       : main()
[Calls]
		setbuf()	//������ ���� ����
	putc()		//char ���
[Given]         : Nothing
[Returns]       : Nothing
*/
void
CharAtaTime(char* str)
{
	char* ptr;
	int		c, i;

	//None buffer, ���۸� ������� �ʰ� �ٷιٷ� ���
	setbuf(stdout, NULL);
	for (ptr = str; c = *ptr++; ) {
		//char �ϳ��� ����ϴµ��� �ð��� �ø�
		//-> time quantum�� �� ���� �ٸ� process�� ����ǵ��� ��
		//-> parent, child process�� �����ư��� �ϳ��� char�� ���
		for (i = 0; i < 999999; i++)
			;
		putc(c, stdout);//char ���
	}
}

/*
[Program Name]  : fork program
[Description]   : fork�� child process�� ����
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process ����
		perror()        //���� �޼��� ���
	exit()          //���α׷� ����
	CharAtaTime()	//�Ķ���ͷ� ���� string�� �� char�� ���
*/
int
main()
{
	pid_t	pid;

	//child process ����
	//parent process��� pid�� child process�� PID ����
	//child process��� pid�� 0 ����
	if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ����
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process�� ���
		CharAtaTime("output from child\n");
	}
	else {//parent process�� ���
		CharAtaTime("output from parent\n");
	}
}
