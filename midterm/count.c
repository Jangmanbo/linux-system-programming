#include <stdio.h>
#include <stdlib.h>


int
main(int argc, char* argv[])
{
	FILE* src;
	char ch;
	int count = 0;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s source destination\n", argv[0]);
		exit(1);
	}

	if ((src = fopen(argv[1], "rt")) == NULL) {
		perror("fopen");
		exit(1);
	}

	printf("%c\n", *argv[2]);

	while ((ch = fgetc(src)) != EOF) {
		if (ch == *argv[2]) {
			count++;
		}
	}

	printf("%d\n", count);
	fclose(src);
}
