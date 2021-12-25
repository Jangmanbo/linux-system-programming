#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int count = 1;
int num = 0;
char str[128] = "Lotto number: ";
void
SigIntHandler(int signo)
{
	if (signo == SIGINT) {
		num++;

		printf("\n%d\n", count);
		char n[128] = "";
		sprintf(n, "%d", count);
		strcat(str, n);
		if (num == 6) {
			printf("%s\n", str);
			exit(0);
		}
		strcat(str, ", ");
	}
}

int
main() {

	signal(SIGINT, SigIntHandler);//SIGINT signal handler µî·Ï	

	while (1) {
		for (int i = 0; i < 44; i++) {
			count++;
			if (count >= 45) {
				count = 1;
			}
		}
	}
}
