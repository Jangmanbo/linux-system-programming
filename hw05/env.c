#include <stdio.h>

/*
[Program Name]  : print envionment variable program
[Description]   : ȯ�溯�� ���
[Input]
		int argc        // argv �迭�� ũ��
		char *argv[]    // status�� ����� ���ϵ��� �迭
	char *envp[]	// ȯ�溯��
[Output]        : ȯ�溯��
[Calls]
	printf()	//�޼��� ���
*/
int
main(int argc, char* argv[], char* envp[])
{
	int			i;
	char** p;
	extern char** environ;	//extern���� ���� �ٸ� �ý��ۿ� �̸� ����� ȯ�溯���� ������

	printf("List command-line arguments\n");
	//�Է¹��� �Ķ���� ���
	for (i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

	printf("\n");
	printf("List environment variables from environ variable\n");

	//C start-up library�� �������� environ���� ȯ�溯���� �о� ���
	//NULL�� ȯ�溯���� ���� �ǹ�
	for (p = environ; *p != NULL; p++) {
		printf("%s\n", *p);
	}

	printf("\n");
	printf("List environment variables from envp variable\n");
	//3���� �Ķ������ envp�� ȯ�溯�� ���
	//NULL�� ȯ�溯���� ���� �ǹ�
	for (p = envp; *p != NULL; p++) {
		printf("%s\n", *p);
	}
}
