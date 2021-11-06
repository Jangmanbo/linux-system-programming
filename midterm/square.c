#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char* argv[]) {
	if (argc != 2) {//argv!=3이면 프로그램 종료
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	int num = atoi(argv[1]);

	printf("%d\n", num * num);
}
