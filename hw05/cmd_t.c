#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

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
	pthread_exit()	//thread ����
[Given]         : Nothing
[Returns]       : Nothing
*/
void
DoCmd(char* cmd)
{
	printf("Doing %s", cmd);
	sleep(1);//1�ʰ� �Ͻ�����
	printf("Done\n");

	pthread_exit(NULL);	//thread ����
}

/*
[Program Name]  : command program
[Description]   : thread�� �����Ͽ� command�� �����ϴ� ��ó�� ����ϴ� ���α׷�
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	perror()        //���� �޼��� ���
	printf()	//�޼��� ���
		exit()          //���α׷� ����
	fgets()		//���� ��Ʈ���� ���ۿ��� ���ڿ��� ����
	DoCmd()		//command ����
	pthread_create()//thread ����
		pthread_join()  //thread�� ����Ǳ⸦ ��ٸ�
*/
int
main()
{
	char		cmd[MAX_CMD];
	pthread_t	tid;

	while (1) {
		printf("CMD> ");

		//shell���� command�� �о��
		fgets(cmd, MAX_CMD, stdin);

		//command�� ù ���ڰ� q�� ���α׷� ����
		if (cmd[0] == 'q')
			break;

		//&tid : ������ thread��id
			//NULL : thread�� attribute
			//DoCmd : thread���� ������ �Լ�
			//cmd : DoCmd �Լ��� �Ķ����
			//DoCmd(cmd)�� �����ϴ� thread ����
		//->���� thread������ command ����� �����ϰ� ���� command�� �Է¹��� �� ����
		if (pthread_create(&tid, NULL, (void*)DoCmd, (void*)cmd) < 0) {//thread ������ �����ϸ� ���α׷� ����
			perror("pthread_create");
			exit(1);
		}

#if 0
		//id�� tid�� thread�� ����� ������ ��ٸ�
		pthread_join(tid, NULL);
#endif
	}
}
