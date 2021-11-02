#include <stdio.h>
#include <string.h>

enum { FALSE, TRUE };

/*
[Function Name] : convert tab to 4 spaces
[Description]	: tab을 4개의 공백으로 변경하여 저장
[Input]
		char	*fname	//tab을 4개의 공백으로 바꿀 파일
[Output]	: Nothing.
[Call By]	: main()
[Calls]         : fopen, fgetc, fputc, fclose
[Given]		: Nothing
[Returns]	: Nothing
*/
void
conv(char* fname)
{
	FILE* src, * dst;
	char	fdest[40];
	int		ch, first;

	//rt:text file을 read
	//fopen : text file을 open, 파일을 가리키는 포인터를 return
	if ((src = fopen(fname, "rt")) == NULL) {//file open에 실패하면 프로그램 종료
		perror("fopen");
		return;
	}

	strcpy(fdest, fname);	//fdest에 fname 복사
	strcat(fdest, ".t");	//fdest에 .t를 이어붙임

	//wt:text file을write
		//fopen : text file을 open, 파일을 가리키는 포인터를 return
	if ((dst = fopen(fdest, "wt")) == NULL) {//file open에 실패하면 프로그램 종료
		perror("fopen");
		return;
	}

	first = TRUE;
	//fgetc : src file pointer로 읽어온 char를 return, 파일을 끝까지 읽으면 EOF를 return
	//파일의 끝까지 읽으면 루프 빠져나옴
	while ((ch = fgetc(src)) != EOF) {
		//읽은 char이 tab키면 공백 4개를 dst file pointer를 통해 파일에 write
		if (first && ch == '\t') {
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
			fputc(' ', dst);
		}
		else { //char이 tab키가 아니면 그대로 write
			fputc(ch, dst);
			//line에서 tab키가 아닌 문자가 처음으로 나왔으면 first를 FALSE로 바꾸어 해당 line에서 더이상 tab키를 4개의 공백으로 바꾸지 않음
			if (first)
				first = FALSE;
			//다음 line으로 넘어가면 다시 tab키를 4개의 공백으로 바꿀 수 있게 함
			if (ch == '\n')
				first = TRUE;
		}
	}

	fclose(src);
	fclose(dst);
}

/*
[Program Name]  : tab to spcae
[Description]   : tab을 공백 4개로 바꾸는 프로그램
[Input]
		int argc        // argv 배열의 크기
		char *argv[]    // 실행할 프로그램, tab을 공백으로 바꿀 파일
[Output]        : Nothing.
[Calls]         : Nothing.
*/

int
main(int argc, char* argv[])
{
	while (--argc) {
		conv(argv[argc]);
	}
}
