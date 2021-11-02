#include <stdio.h>
#include <unistd.h>
#define	MAX_CMD		256

/*
[Function Name] : DoCmd
[Description]   : �Է¹��� command�� ������ ��ó�� ���
[Input]
	char*	cmd	//������ command
[Output]        : command�� �����ߴٴ� �޼���
[Call By]       : main()
[Calls]
		printf()        //�޼��� ���
	sleep()		//�Է¹��� �ð����� �Ͻ�����
[Given]         : Nothing
[Returns]       : Nothing
*/
void
DoCmd(char* cmd)
{
	printf("Doing %s", cmd);
	sleep(1);//1�ʰ� �Ͻ�����
	printf("Done\n");
}

/*
[Program Name]  : command program
[Description]   : command�� �����ϴ� ��ó�� ����ϴ� ���α׷�
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	printf()	//�޼��� ���
	fgets()		//���� ��Ʈ���� ���ۿ��� ���ڿ��� ����
	DoCmd()		//command ����
*/
int
main()
{
	char	cmd[MAX_CMD];

	while (1) {
		printf("CMD> ");
		//shell���� command�� �о��
		fgets(cmd, MAX_CMD, stdin);
		//command�� ù ���ڰ� q�� ���α׷� ����	
		if (cmd[0] == 'q')
			break;

		//command ����
		//command ������ ���� ������ ���� command�� �Է¹��� ����
		DoCmd(cmd);
	}
}
