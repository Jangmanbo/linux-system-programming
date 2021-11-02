#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int
mysystem(char* cmd) {
	int	status;
	pid_t	pid;
	char* argv[] = { "sh", "-c", cmd, NULL };

	//child process ����
		//parent process��� pid�� child process�� PID ����
		//child process��� pid�� 0 ����
	if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ����
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {//child process�� ���
		if (execv("/bin/sh", argv) < 0) {//���� ������ ���丮 ��ü ��ο� argv �迭�� ���� �ٸ� ���α׷��� ����, �����ϸ� ���α׷� ����
			perror("execv");
			exit(1);
		}
	}

	//process id�� pid�� process�� ���Ḧ ��ٸ�, �����ϸ� ���α׷� ����
	if (waitpid(pid, &status, 0) < 0) {
		perror("waitpid");
		exit(1);
	}

	//command ���࿡ �����ϸ� command�� return value�� return
	//�����ϸ� -1 return
	return status;
}

/*
[Program Name]  : system program
[Description]   : mysystem �Լ��� ȣ���Ͽ� command ����(child process�� command�� �����ϰ� child�� ���� ������ ��ٷȴٰ� ������ ���� command ����)
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
	if ((status = mysystem("date")) < 0) {//�����ϸ� ���α׷� ����
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//nosuchcommand��� command�� �������� �����Ƿ� status�� -1�� ����, ���α׷� ����
	if ((status = mysystem("nosuchcommand")) < 0) {
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//status�� who�� exit 44 �� command�� return���� ����
	if ((status = mysystem("who; exit 44")) < 0) {//�����ϸ� ���α׷� ����
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

	//mycp ���α׷� ���� (�߰� ����)
	if ((status = mysystem("./mycp exec.c exec2.c")) < 0) {//�����ϸ� ���α׷� ����
		perror("system");
		exit(1);
	}
	printf("exit status = %d\n", status);

}
