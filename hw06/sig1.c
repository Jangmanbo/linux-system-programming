#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

/*
[Function Name] : SigIntHandler
[Description]   : SIGINT signal�� �߻��ϸ� �޽��� ��� �� ���α׷� ����
[Input]         : signo	//������ signal
[Output]        : ��� �޼���
[Call By]	: kernel
[Calls]
	printf()	//�޼��� ���
	exit()		//���α׷� ����
[Given]		: Nothing.
[Returns]	: Nothing.
*/
void
SigIntHandler(int signo)
{
	printf("Received a SIGINT signal\n");
	printf("Terminate this process\n");

	exit(0);
}
/*
[Program Name]  : signal program
[Description]   : SIGINT signal handler�� ����ϰ� signal�� �߻��� ������ ���
[Input]         : Nothing.
[Output]        : Nothing.
[Calls]
		signal()        //signal handler�� ���
	printf()	//�޼��� ���
	pause()		//signal�� �߻��� ������ ���
*/
int
main()
{
	signal(SIGINT, SigIntHandler);//SIGINT signal handler ���

	printf("Press ^C to quit\n");

	for (; ; )
		pause();	//signal�� �߻��� ������ �ٸ� ���� ���� �ʰ� ��ٸ�
}
