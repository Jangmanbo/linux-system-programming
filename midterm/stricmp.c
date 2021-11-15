#include <stdio.h>
#include <stdlib.h>

/*
[Function Name] : stricmp_p
[Description]   : ��ҹ��� ���� ���� �� ���ڿ��� ���Ͽ� ����� ����
[Input]         :
	char	*dst	//ù��° ���ڿ�
	char	*src	//�ι�° ���ڿ�
[Output]        : ���ڿ� �� ���(ù��° ���ڿ��� ũ�� 1, ������ -1, �� ���ڿ��� �����ϸ� 0)
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
		//�� ���ڿ��� ���ڰ� �ҹ����� ��� �빮�ڷ� ����
		if (*dst <= 122 && *dst >= 97) *dst -= 32;
		if (*src <= 122 && *src >= 97) *src -= 32;
		//���� dst �����Ͱ� ����Ű�� char�� src �����Ͱ� ����Ű�� char���� �ƽ�Ű�ڵ尡 ũ�� 1��return
		if (*dst > *src) return 1;
		//���� src �����Ͱ� ����Ű�� char�� dst �����Ͱ� ����Ű�� char���� �ƽ�Ű�ڵ尡 ũ�� 1��return
		else if (*dst < *src) return -1;
		else {
			//�� ���ڿ��� ��� ���� ���=�� ���ڿ��� ����->return 0
			if (*dst == '\0') return 0;
			//���� ���ڿ��� ������ �ʾ����� ��� loop
		}

		//�� �����Ϳ� 4�� ���� ���� char�� ����Ű���� ��
		dst++, src++;
	};
}
/*
[Program Name]  : string compare program
[Description]   : ��ҹ��� ���� ���� ���ڿ��� ���ϴ� ���α׷�
[Input]         : Nothing.
[Output]        : ��ҹ��� �� ���
[Calls]
	printf()	//�޼��� ���
*/
void main()
{

	char str1[] = "Hello";

	char str2[] = "hello";

	char str3[] = "World!";

	printf("%d : %d\n", stricmp_p(str1, str2), stricmp_p(str1, str3));

}
