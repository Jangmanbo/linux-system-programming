#include <stdio.h>

int
strlen_p(char* str)
{
	int	len = 0;

	//If the char that the pointer points to is null, repeat it.
	//Add 4 to the pointer to point to the next char.
	while (*(str++) != '\0') {
		len++; //Add 1 to len.
	}
	return len; //return the length of str
}

int
strlen_a(char str[])
{
	int i;

	//If the element of the current index is not null, repeat it.
	//The index increases by 1 every time you repeat it.
	for (i = 0; str[i] != '\0'; i++)
		;
	//At the end of the for loop, i is the index of the element containing the first null.
	//Since the index starts at 0, i is the length of the str.
	return i; //return the length of str
}

void
strcpy_p(char* dst, char* src)
{
	//If the char that src pointer points to is null, repeat it.
	while (*src) {
		//Substituting char indicated by src pointer into the memory space indicated by dst pointer.
		//Add 4 to each of the two pointers(dst, src) to point to the next char.
		*dst++ = *src++;
	}
	//At the end of the while loop, src pointer points to null.
	//Indicate that the string is over by assignmenting null into the memory space currently indicated by the dst pointer.
	*dst = *src;
}

void
strcpy_a(char dst[], char src[])
{
	int i;

	//Repeat if the element of the current index of src array is not null.
	//The index increases by 1 every time you repeat it.
	for (i = 0; src[i] != '\0'; i++)
		//assignmenting the value of src[i] into dst[i]
		dst[i] = src[i];
	//At the end of the for loop, i is the index of the element containing the first null. (src arary)
	//Indicate that the string is over by assignmenting null into dst[i].
	dst[i] = src[i];
}

void
strcat_p(char* dst, char* src)
{
	//If the char that dst pointer points to is null, repeat it.
	//Add 4 to dst pointer to point to the next char.
	while (*dst++)
		;
	//Because dst pointer points to null at the end of while loop, subtract 4 to the dst pointer to point to the first null.
	dst--;

	//If the char that src pointer points to is null, repeat it.
	while (*src) {
		//assignmenting char indicated by src pointer into the memory space indicated by dst pointer.
		//Add 4 to each of the two pointers(dst, src) to point to the next char.
		//That is, starting from the first null of dst, assignmenting the values of src in order.
		*dst++ = *src++;
	}
	//At the end of the while loop, src pointer points to null.

	//Indicate that the string is over by assignmenting null into the memory space currently indicated by the dst pointer.
	*dst = *src;
}

void
strcat_a(char dst[], char src[])
{
	int i, j;

	//Repeat if the element of the current index of dst array is not null.
	//The index increases by 1 every time you repeat it.
	for (i = 0; dst[i] != '\0'; i++)
		;
	//At the end of the for loop, i is the index of the element containing the first null. (dst arary)

	//Repeat if the element of the current index of src array is not null.
	//The index increases by 1 every time you repeat it.
	for (j = 0; src[j] != '\0'; j++)
		//Assignment the value of the first element of src into the element containing the first null of the dst array.
		//Thereafter, assignment the value of the next element of src into the next element of the dst array.
		dst[i + j] = src[j];
	//At the end of the for loop, j is the index of the element containing the first null. (src arary)

	//Indicate that the string is over by assignmenting null into dst[i + j].
	dst[i + j] = src[j];
}

int
strcmp_p(char* dst, char* src)
{
	//If the char that dst pointer points to and the char that src pointer points to same, repeat it.
	while (*dst == *src) {
		//If the char that dst pointer points to is null, it means that both strings are finished.
		//In other words, the two strings are the same, so return 0.
		if (*dst == '\0') return 0;
		//Add 4 to each of the two pointers(dst, src) to point to the next char.
		dst++, src++;
	}
	//If the char that dst pointer points to is bigger than the char that src pointer points to, return 1;
	if (*dst - *src > 0) return 1;
	//If the char that dst pointer points to is smaller than the char that src pointer points to, return -1;
	else return -1;
}

int
strcmp_a(char dst[], char src[])
{
	int i;
	//Repeat if the element of the current index of dst array is not null and it's same with the element of the current index of src array.
	//The index increases by 1 every time you repeat it.
	for (i = 0; dst[i] == src[i] && dst[i]; i++)
		;
	//At the end of the for loop, i is the index of the element containing the first null of dst or src array, or the index of the element whose two strings change for the first time.

	//If the element of the current index of the dst array is larger than the element of the current index of the src array, return 1.
	if (dst[i] - src[i] > 0) return 1;
	//If the element of the current index of the dst array is smaller than the element of the current index of the src array, return -1.
	else if (dst[i] - src[i] < 0) return -1;
	//If the element of the current index of the dst array and the element of the current index of the src array are the same, return 0.
	else return 0;
}

int
main()
{
	int		len1, len2;
	char	str1[20], str2[20];

	len1 = strlen_p("Hello");
	len2 = strlen_a("Hello");
	printf("strlen: p=%d, a=%d\n", len1, len2);

	strcpy_p(str1, "Hello");
	strcpy_a(str2, "Hello");
	printf("strcpy: p=%s, a=%s\n", str1, str2);

	strcat_p(str1, ", World!");
	strcat_a(str2, ", World!");
	printf("strcat: p=%s, a=%s\n", str1, str2);

	//strcmp_p
	printf("strcmp_p(%s, %s) = %d\n", str1, "Hello", strcmp_p(str1, "Hello")); //The front part of the string is the same, but the length is different.
	printf("strcmp_p(%s, %s) = %d\n", str1, "hello", strcmp_p(str1, "hello")); //The strings are different.
	printf("strcmp_p(%s, %s) = %d\n", str1, "Hello, World!", strcmp_p(str1, "Hello, World!")); //The two strings are the same.

	//strcmp_a
	printf("strcmp_a(%s, %s) = %d\n", str1, "Hello", strcmp_p(str1, "Hello")); //The front part of the string is the same, but the length is different.
	printf("strcmp_a(%s, %s) = %d\n", str1, "hello", strcmp_p(str1, "hello")); //The strings are different.
	printf("strcmp_a(%s, %s) = %d\n", str1, "Hello, World!", strcmp_p(str1, "Hello, World!")); //The two strings are the same.



}
