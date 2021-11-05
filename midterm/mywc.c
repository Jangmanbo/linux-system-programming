#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

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
		char* args[4] = { "wc", "-w", argv[1], NULL };
		if (execvp("wc", args) < 0) {
			perror("execvp");
			exit(1);
		}
	}
	else {//parent process�� ���
		//&status : child process�� exit�� return value(child process�� ����� �� status)
		//child process�� ����� ������ ��ٸ� �� child process�� ����Ǿ����� ���
		wait(NULL);
	}





}
