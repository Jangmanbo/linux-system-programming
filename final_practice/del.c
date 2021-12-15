#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void del(char str[])
{
	strncpy(str, str + 1, strlen(str));
}

//입력받은 문자열 첫글자 삭제
int
main()
{
	char str[20] = "Hello";
	del(str);
	puts(str);
}
