#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

/*
[Function Name] : myexit1
[Description]   : print message
[Input]		: Nothing.
[Output]        : Nothing.
[Call By]       : main()
[Calls]
	printf()	//�޼��� ���
[Given]         : Nothing
[Returns]       : Nothing
*/
void
myexit1()
{
	printf("first exit handler\n");
}

/*
[Function Name] : myexit2
[Description]   : print message
[Input]         : Nothing.
[Output]        : Nothing.
[Call By]       : main()
[Calls]
		printf()        //�޼��� ���
[Given]         : Nothing
[Returns]       : Nothing
*/
void
myexit2()
{
	printf("second exit handler\n");
}

/*
[Program Name]  : atexit program
[Description]   : ateexit system call�� ȣ���Ͽ� exit handler�� ���
[Input]		: Nothing.
[Output]        : Nothing.
[Calls]
		perror()        //���� �޼��� ���
	printf()	//�޼��� ���
		exit()          //���α׷� ����
	atexit()	//exit handler ���
*/
int
main()
{
	if (atexit(myexit2) != 0) {//myexit2 �Լ��� exit handler�� ���, �����ϸ� ���α׷� ����
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0) {//myexit1 �Լ��� exit handler�� ���, �����ϸ� ���α׷� ����
		perror("atexit");
		exit(1);
	}

	if (atexit(myexit1) != 0) {//myexit1 �Լ��� exit handler�� ���, �����ϸ� ���α׷� ����
		perror("atexit");
		exit(1);
	}

	//main�Լ��� ����Ǿ����� ���
	printf("main is done\n");
	//exit handler ����
}
