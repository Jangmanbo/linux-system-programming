#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

/*
[Program Name]  : program execute
[Description]   : child process�� �����Ͽ� ���� ������� �ٸ� ���α׷��� ����
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process ����
		perror()        //���� �޼��� ���
		exit()          //���α׷� ����
	execl()		//argv�� list�� �޾� ���α׷� ����
	execv()		//argv�� vector�� �޾� ���α׷� ����
	execlp()	//path�� ����� ��ο��� ���������� ã�Ƽ� ����
	waitpid()	//PID�� pid�� child process�� ����� ������ ��ٸ�
*/
int
main()
{
	pid_t	pid;
	char* argv[] = { "mycp", "exec.c", "exec2.c", NULL };

	//child process ����
	//parent process��� pid�� child process�� PID ����
	//child process��� pid�� 0 ����
	if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ����
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process�� ���
		//execl ������� mycp ���α׷� ����
		//NULL�� argv list�� ������ ǥ��
		if (execl("/home/lsp36/practice/hw05/mycp",
			"mycp", "exec.c", "exec1.c", NULL) < 0) {//�����ϸ� -1 return, ���α׷� ����
			perror("execl");
			exit(1);
		}
	}
	//PID�� pid�� child process�� ����� ������ ��ٸ�
	if (waitpid(pid, NULL, 0) < 0) {
		perror("waitpid");
		exit(1);
	}

	//child process ����
		//parent process��� pid�� child process�� PID ����
		//child process��� pid�� 0 ����
	if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ����
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process�� ���
			//execv ������� mycp ���α׷� ����
				//�̸� ����� argv �迭 ���
		if (execv("/home/lsp36/practice/hw05/mycp", argv) < 0) {//�����ϸ� -1 return, ���α׷� ����
			perror("execv");
			exit(1);
		}
	}

	//PID�� pid�� child process�� ����� ������ ��ٸ�
	if (waitpid(pid, NULL, 0) < 0) {
		perror("waitpid");
		exit(1);
	}

	//child process ����
		//parent process��� pid�� child process�� PID ����
		//child process��� pid�� 0 ����
	if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ����
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process�� ���
				//execlp
		//path���� env�� ã�� ����
		if (execlp("env", "env", NULL) < 0) {
			perror("execlp");
			exit(1);
		}
	}

	//PID�� pid�� child process�� ����� ������ ��ٸ�
	if (waitpid(pid, NULL, 0) < 0) {
		perror("waitpid");
		exit(1);
	}
}
