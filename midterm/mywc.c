#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

/*
[Program Name]  : wc -w program
[Description]   : word ������ ����ϴ� wc -w command�� ���� ����� �����ϴ� ���α׷�
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // ���ϰ� ����
[Output]        : argv[1] ���Ͽ�argv[2]�� �� �� �ִ���
[Calls]         :
	fprintf()	//���Ͽ� �޼��� ���
	exit()		//���μ��� ����
	perror()	//���� �޼��� ���
	execvp()	//path�� ����� ��ο��� ���������� ã�Ƽ� ����
	wait()		//child process�� ����� ������ ���
*/
int
main(int argc, char* argv[])
{
	int pid;

	if (argc != 2) {//argv!=3�̸� ���α׷� ����
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ����
		perror("fork");
		exit(1);
	}
	else if (pid == 0) {//child process�� ���
		//execvp �μ� �迭 ����
		char* args[4] = { "wc", "-w", argv[1], NULL };
		//path�� ����� ��ο��� ���������� ã�Ƽ� ����
		if (execvp("wc", args) < 0) {
			//�����ϸ� ���μ��� ����
			perror("execvp");
			exit(1);
		}
	}
	else {//parent process�� ���
		//child process�� ����� ������ ���
		wait(NULL);
	}

}
