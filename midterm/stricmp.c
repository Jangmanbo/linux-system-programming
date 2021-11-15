#include <stdio.h>
#include <stdlib.h>

/*
[Function Name] : stricmp_p
[Description]   : 대소문자 구분 없이 두 문자열을 비교하여 결과를 리턴
[Input]         :
	char	*dst	//첫번째 문자열
	char	*src	//두번째 문자열
[Output]        : 문자열 비교 결과(첫번째 문자열이 크면 1, 작으면 -1, 두 문자열이 동일하면 0)
[Call By]       : main()
[Calls]         : Nothing.
[Given]         : Nothing.
[Return]        : Nothing.
*/
int
stricmp_p(char* dst, char* src)
{
	while (1)
	{
		//두 문자열의 문자가 소문자일 경우 대문자로 변경
		if (*dst <= 122 && *dst >= 97) *dst -= 32;
		if (*src <= 122 && *src >= 97) *src -= 32;
		//현재 dst 포인터가 가리키는 char이 src 포인터가 가리키는 char보다 아스키코드가 크면 1을return
		if (*dst > *src) return 1;
		//현재 src 포인터가 가리키는 char이 dst 포인터가 가리키는 char보다 아스키코드가 크면 1을return
		else if (*dst < *src) return -1;
		else {
			//두 문자열이 모두 끝난 경우=두 문자열이 동일->return 0
			if (*dst == '\0') return 0;
			//아직 문자열이 끝나지 않았으면 계속 loop
		}

		//두 포인터에 4를 더해 다음 char를 가리키도록 함
		dst++, src++;
	};
}
/*
[Program Name]  : string compare program
[Description]   : 대소문자 구분 없이 문자열을 비교하는 프로그램
[Input]         : Nothing.
[Output]        : 대소문자 비교 결과
[Calls]
	printf()	//메세지 출력
*/
void main()
{

	char str1[] = "Hello";

	char str2[] = "hello";

	char str3[] = "World!";

	printf("%d : %d\n", stricmp_p(str1, str2), stricmp_p(str1, str3));

}
