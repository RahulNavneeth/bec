#include <stdio.h>
#include <stdint.h>
#include "bec.h"

int32_t main(int argc, char **argv) {
	const char *input = "l6:yellow8:umbrellai69ee";
	const char *p = input;
	Bec* b = bec_parse(&p);
	if (b == NULL) {
		printf("\033[31m" "Failed to parse the input : %s\n", input);
		return 1;
	}
	printf("%u : %ld\n", b->type, b->list.count);
	return 0;
}
