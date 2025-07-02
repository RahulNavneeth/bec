#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "bec.h"

char bec_get (const char **input) {
	return (**input);
}

char bec_next (const char **input) {
	(*input)++;
	return (**input);
}

long parse_int (const char **input) {
	long value = 0;
	int sign = 1;
	if (bec_get(input) == '-') {
		sign = -1;
		bec_next(input);
	}
	while (isdigit (bec_get(input))) {
		value = (value * 10) + (bec_get (input) - '0');
		bec_next(input);
	}
	return value * sign;
} 

Bec* bec_parse_int(const char **input) {
	bec_next(input);
	Bec *b = (Bec *)malloc(sizeof(Bec));
	b->type = BEC_INT;
	b->integer = parse_int(input);
	if (bec_get(input) != 'e') {
		free(b);
		return NULL;
	}
	return b;
}

Bec* bec_parse (const char **input) {
	if (bec_get(input) == 'i') return bec_parse_int(input);
	return NULL;
}
