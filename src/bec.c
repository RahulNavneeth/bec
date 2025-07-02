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

Bec* parse_list(const char **input) {
	Bec *b = (Bec*)malloc(sizeof(Bec));
	b->type = BEC_LIST;
	Bec **items = NULL;
	int count = 0; 
	char curr = next(input);
	while (curr != 'e' && curr != ' ') {
		Bec* rb = bec_parse(input);
		if (rb == NULL) return NULL;
		count++;
		items = realloc(items, count * sizeof(*items));
		items[count-1] = rb;
		curr = next(input);
	}
	if (curr != 'e') {
		free(b); free(b->list.items);
		return NULL;
	}
	b->list.count = count;
	b->list.items = items;
	return b;
}

Bec* bec_parse (const char **input) {
	printf("curr :: %c\n", get(input));
	if (get(input) == 'i') return parse_int(input);
	if (isdigit(get(input))) return parse_string(input);
	if (get(input) == 'l') return parse_list(input);
	return NULL;
}
