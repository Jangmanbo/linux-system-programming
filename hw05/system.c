#include <stdio.h>
#include <stdlib.h>

/*
[Program Name]  : system program
[Description]   : system�̶�� system call�� ȣ���Ͽ� command ����(child process�� command�� �����ϰ� child�� ���� ������ ��ٷȴٰ� ������ ���� command ����)
[Input]		: Nothing.
[Output]        : command ���� ���
[Calls]
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
	system()	//�Ķ���ͷ� ���� ��ɾ ����, ��ɾ��� return���� return, �����ϸ� -1 return
	printf		//�޼��� ���
*/
int
main()
{
	int		status;

	//status�� date command�� return���� ����
	if ((status = system("date")) < 0) {//�����ϸ� ���α׷� ����
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//nosuchcommand��� command�� �������� �����Ƿ� status�� -1�� ����, ���α׷� ����
	if ((status = system("nosuchcommand")) < 0) {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//status�� who�� exit 44 �� command�� return���� ����
	if ((status = system("who; exit 44")) < 0) {//�����ϸ� ���α׷� ����
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);
}
