#include <stdio.h>
#include <stdint.h>
#include "bec.h"

int32_t main(int argc, char **argv) {
	const char *input = "d5:effiel6:yellowd5:effiel6:yellow8:umbrellai69ee6:claire5:trayce8:umbrellai69ee6:claire5:trayce";
	const char *p = input;
	Bec* b = bec_parse(&p);
	if (b == NULL) {
		printf("\033[31m" "Failed to parse the input : %s\n", input);
		return 1;
	}
	bec_print(b, 0);
	bec_clean(b);
	return 0;
}
