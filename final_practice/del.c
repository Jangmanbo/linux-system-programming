#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void del(char str[])
{
	strncpy(str, str + 1, strlen(str));
}

//�Է¹��� ���ڿ� ù���� ����
int
main()
{
	char str[20] = "Hello";
	del(str);
	puts(str);
}
