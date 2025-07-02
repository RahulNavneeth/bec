#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "bec.h"

char get (const char **input) {
	return (**input);
}

char next (const char **input) {
	(*input)++;
	return (**input);
}

long get_int_value (const char **input) {
	long value = 0;
	int sign = 1;
	if (get(input) == '-') {
		sign = -1;
		next(input);
	}
	while (isdigit (get(input))) {
		value = (value * 10) + (get (input) - '0');
		next(input);
	}
	return value * sign;
} 

Bec* parse_int(const char **input) {
	next(input);
	Bec *b = (Bec *)malloc(sizeof(Bec));
	b->type = BEC_INT;
	b->integer = get_int_value(input);
	if (get(input) != 'e') {
		free(b);
		return NULL;
	}
	return b;
}

Bec *parse_string(const char **input) {
	Bec *b = (Bec*)malloc(sizeof(Bec));
	b->type = BEC_STRING;
	long len = get_int_value(input);  
	b->string.len = len;
	if (get(input) != ':') {
		free(b);
		return NULL;
	}
	char *str = (char*)malloc(sizeof(char)*b->string.len);
	for (size_t i = 0 ; i < b->string.len ; i++) {
		str[i] = next(input);
	}
	b->string.str = str;
	return b;
}

Bec* bec_parse (const char **input) {
	if (get(input) == 'i') return parse_int(input);
	if (isdigit(get(input))) return parse_string(input);
	return NULL;
}
