#include <stdio.h>
#include <stdint.h>
#include "bec.h"

int32_t main(int argc, char **argv) {
	const char *input = "i-39433";
	const char *p = input;
	Bec* b = bec_parse(&p);
	if (b == NULL) {
		printf("\033[31m" "Failed to parse the input\n");
		return 1;
	}
	printf("%u : %ld\n", b->type, b->integer);
	return 0;
}
