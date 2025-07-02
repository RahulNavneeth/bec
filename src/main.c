#include <stdio.h>
#include <stdint.h>
#include "bec.h"

int32_t main(int argc, char **argv) {
	const char *input = "6:yellow";
	const char *p = input;
	Bec* b = bec_parse(&p);
	if (b == NULL) {
		printf("\033[31m" "Failed to parse the input\n");
		return 1;
	}
	printf("%u : %ld %s\n", b->type, b->string.len, b->string.str);
	return 0;
}
