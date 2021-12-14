#include <stdio.h>
#include <signal.h>
#include <unistd.h>


/*
[Function Name]  : SigAlarmHandler
[Description]   : SIGALRM�� �߻��Ǹ� ȣ��
[Input]         : signo	//������ signal
[Output]        : Nothing.
[Call By]	: kernel
[Calls]		: Nothing.
[Given]		: Nothing.
[Return]	: Nothing.
*/
void
SigAlarmHandler(int signo)
{
	/* nothing to do, just return to wake up the pause */
	return;
}

/*
[Function Name] : mysleep
[Description]   : SIGALRM signal handler�� ����ϰ� 5�� �Ŀ� SIGALRM�� ����
[Input]         :
	nsecs	//���� �Ŀ� SIGALRM�� ������
[Output]        : Nothing.
[Call By]       : main()
[Calls]         :
	signal()        //signal handler�� ���
		alarm()         //n�� �Ŀ� SIGALRM�� ����
	pause()		//signal�� �߻��� ������ ���
[Given]         : Nothing.
[Return]        : �ֱٿ� ������ alarm�� ���� ���Ҵ���
*/
unsigned int
mysleep(unsigned int nsecs)
{
	//SIGALRM signal handler ���
	if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR) {
		return nsecs;
	}

	alarm(5);	//5�� �� SIGALRM signal ����

	pause();	//signal�� �߻��� ������ ���

	//�ֱٿ� ������ alarm�� �� �� ���Ҵ���return
	//SIGALRM�� �߻��ϱ� ������ �ٸ� signal�� �����ٸ� 0�� return
	//SIGALRM�� �߻��ϱ� ���� n�� �� �ٸ� signal�� �߻��ϸ� 5-n�� return
	return alarm(0);
}
/*
[Program Name]  : signal program
[Description]   : 5�� �� SIGALRM�� ���� ���μ��� ����
[Input]         : Nothing.
[Output]        : �ȳ� �޼���
[Calls]
	printf()	//�޼��� ���
	mysleep()	//SIGALRM signal handler�� ����ϰ� 5�� �Ŀ� SIGALRM�� ����
*/
int
main()
{
	printf("Wait for 5 seconds...\n");


	//SIGALRM signal handler�� ����ϰ� 5�� �Ŀ� SIGALRM�� ����
	mysleep(5);
}
