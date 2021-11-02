#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

//��������
int		Var = 6;
char	Buf[] = "a write to stdout\n";

/*
[Program Name]  : fork program
[Description]   : fork�� child process�� ����
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
	fork()		//child process ����
		perror()        //���� �޼��� ���
	printf()	//�޼��� ���
		exit()          //���α׷� ����
	write()		//���Ͽ� ������ ����
	sleep()		//������ �ð����� ���
	getpid()	//�Լ��� ȣ���� ���μ����� PID��return
*/
int
main()
{
	//���ú���
	int		var;
	pid_t	pid;

	var = 88;
	write(STDOUT_FILENO, Buf, sizeof(Buf) - 1);//STDOUT_FILENO�� Buf ���, �� printf�� ����
	printf("Before fork\n");

	//child process ����
	//parent process��� pid�� child process�� PID ����
	//child process��� pid�� 0 ����
	if ((pid = fork()) < 0) {//fork�� return value�� �����̸� error, ���α׷� ����
		perror("fork");
		exit(1);
	}

	else if (pid == 0) {//child process�� ���
		/* child */
		//child process�� Var, var ������ �� ����
		Var++;
		var++;
	}
	else {//parent process�� ���
		/* parent */
		sleep(2);//parent process�� 2�ʵ��� ���
	}

	//�ڽ��� PID�� Var, var������ �� ���
	//child process�� ��� Var=7, var=89�� ���
	//parent process�� ��� Var, var�� ���� ������ �����Ƿ�Var=6, var=88�� ���
	//parent process�� 2�ʰ� ����ϹǷ� child process�� ���� ���
	printf("pid = %d, Var = %d, var = %d\n", getpid(), Var, var);
}
