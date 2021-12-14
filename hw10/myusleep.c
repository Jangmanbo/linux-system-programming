#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

/*
[Function Name] : myusleep
[Description]   : us ������ sleep
[Input]
	int	secs	//s ����
	int	usecs	//us ����
[Output]        : Nothing.
[Call By]       : main
[Calls]
		printf()        //�޼��� ���
	select()	//���� ���� socket�� ó��(�� �Լ������� �ܼ��� sleep�ϴ� �뵵)
[Given]		: Nothing.
[Returns]       : Nothing.
*/
void
myusleep(int secs, int usecs)
{
	struct timeval	tm;	//��� �ð�(������� ��ŭ �ڴ���)

	tm.tv_sec = secs;	//s ����
	tm.tv_usec = usecs;	//us ����

	//tm�� ����� �ð���ŭ sleep
	if (select(0, (fd_set*)0, (fd_set*)0, (fd_set*)0, &tm) < 0) {
		perror("select");
		return;
	}
}

/*
[Program Name]  : myusleep program
[Description]   : 1.5�ʰ� sleep �� ����
[Input]         : Nothing.
[Output]        : ���ʰ� �ڴ��� ���
[Calls]         :
		printf()        //�޼��� ���
	myusleep()	//���ϴ� �ð����� sleep
*/
int
main()
{
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);
}
