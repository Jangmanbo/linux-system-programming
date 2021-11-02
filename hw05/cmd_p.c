#include <stdio.h>
#include <stdlib.h>
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
[Description]   : child process�� �����Ͽ� command�� �����ϴ� ��ó�� ����ϴ� ���α׷�
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()          //child process ����
	perror()        //���� �޼��� ���
	printf()	//�޼��� ���
		exit()          //���α׷� ����
	fgets()		//���� ��Ʈ���� ���ۿ��� ���ڿ��� ����
	DoCmd()		//command ����
*/
int
main()
{
	char	cmd[MAX_CMD];
	int		pid;

	while (1) {
		printf("CMD> ");
		//shell���� command�� �о��
		fgets(cmd, MAX_CMD, stdin);
		//command�� ù ���ڰ� q�� ���α׷� ����
		if (cmd[0] == 'q')
			break;

		//child process ����
		//parent process��� pid�� child process�� PID ����
		//child process��� pid�� 0 ����
		if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ����
			perror("fork");
			exit(1);
		}

		//child process�� command�� ����
				//-> parent process�� command ����� �����ϰ� ���� command�� �Է¹��� �� ����
		else if (pid == 0) {
			DoCmd(cmd);//command ����
			exit(0);//process ����
		}
#if 0
		else {//parent process�� ���
			//child process�� ���Ḧ ��ٸ�
			wait(NULL);
		}
#endif
	}
}
