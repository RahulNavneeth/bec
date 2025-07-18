#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
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
 		Bec* rb = bec_decode(input);
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

Bec* parse_dict(const char **input) {
	Bec* b = (Bec*)malloc(sizeof(Bec));
	b->type = BEC_DICT;
	char curr = next(input);
	long count = 0;
	Bec **keys = NULL;
	Bec **values = NULL;
	while (curr != 'e' && curr != ' ') {
		count ++;
		Bec* key = parse_string(input);
		if (key == NULL) {
			free(b); return NULL;
		}
		keys = realloc(keys, count * sizeof(*keys));
		next(input);
		keys[count-1] = key;
		Bec* value = bec_decode(input);
		if (value == NULL) {
			free(b); return NULL;
		}
		values = realloc(values, count * sizeof(*values));
		values[count-1] = value;
		curr = next(input);
	}
	if (curr != 'e') {
		free(b);
		free(keys);
		free(values);
		return NULL;
	}
	b->dict.keys = keys;
	b->dict.values = values;
	b->dict.count = count;
	return b;
}

Bec* bec_decode (const char **input) {
	if (get(input) == 'i') return parse_int(input);
	if (isdigit(get(input))) return parse_string(input);
	if (get(input) == 'l') return parse_list(input);
	if (get(input) == 'd') return parse_dict(input);
	return NULL;
}

void bec_print(Bec* b, int indent) {
    if (b == NULL) {
        for (int i = 0; i < indent; i++) printf("  ");
        printf("\033[0;31m[NULL]\033[0m\n");
        return;
    }
    for (int i = 0; i < indent; i++) printf("  ");
    
    switch (b->type) {
        case BEC_INT:
            printf("\033[0;32m%ld\033[0m\n", b->integer);
            break;
            
        case BEC_STRING: {
            printf("\033[0;33m[%d bytes] ", b->string.len);
            
            int is_printable = 1;
            for (int i = 0; i < b->string.len; i++) {
                unsigned char c = (unsigned char)b->string.str[i];
                if (c < 32 && c != '\n' && c != '\r' && c != '\t') {
                    is_printable = 0;
                    break;
                }
            }
            
            if (is_printable && b->string.len < 200) {
                printf("\"");
                for (int i = 0; i < b->string.len; i++) {
                    char c = b->string.str[i];
                    if (c == '\n') printf("\\n");
                    else if (c == '\r') printf("\\r");
                    else if (c == '\t') printf("\\t");
                    else if (c == '"') printf("\\\"");
                    else if (c == '\\') printf("\\\\");
                    else printf("%c", c);
                }
                printf("\"");
            } else {
                printf("(hex) ");
                int show_bytes = b->string.len < 32 ? b->string.len : 32;
                for (int i = 0; i < show_bytes; i++) {
                    printf("%02x", (unsigned char)b->string.str[i]);
                    if (i < show_bytes - 1) printf(" ");
                }
                if (b->string.len > 32) printf("...");
            }
            printf("\033[0m\n");
            break;
        }
        
        case BEC_LIST:
            printf("\033[0;36mList[%zu items]:\033[0m\n", b->list.count);
            for (size_t i = 0; i < b->list.count; i++) {
                for (int j = 0; j < indent + 1; j++) printf("  ");
                printf("[%zu] ", i);
                bec_print(b->list.items[i], 0);
            }
            break;
            
        case BEC_DICT:
            printf("\033[0;35mDict[%zu items]:\033[0m\n", b->dict.count);
            for (size_t i = 0; i < b->dict.count; i++) {
                for (int j = 0; j < indent + 1; j++) printf("  ");
                printf("Key: ");
                bec_print(b->dict.keys[i], 0);
                for (int j = 0; j < indent + 1; j++) printf("  ");
                printf("Val: ");
                bec_print(b->dict.values[i], indent + 1);
            }
            break;
            
        default:
            printf("\033[0;31mUnknown type: %d\033[0m\n", b->type);
            break;
    }
}

char* bec_encode(Bec* b) {
	char* res = NULL;

	switch (b->type) {
		case BEC_INT: {
			int len = snprintf(NULL, 0, "i%lde", b->integer);
			res = malloc(len + 1);
			sprintf(res, "i%lde", b->integer);
			break;
		}
		case BEC_STRING: {
			int len = snprintf(NULL, 0, "%d:%s", b->string.len, b->string.str);
			res = malloc(len + 1);
			sprintf(res, "%d:%s", b->string.len, b->string.str);
			break;
		}
		case BEC_LIST: {
			res = strdup("l");
			for (size_t i = 0; i < b->list.count; i++) {
				char* enc = bec_encode(b->list.items[i]);
				size_t new_len = strlen(res) + strlen(enc) + 1;
				res = realloc(res, new_len + 1);
				strcat(res, enc);
				free(enc);
			}
			res = realloc(res, strlen(res) + 2);
			strcat(res, "e");
			break;
		}
		case BEC_DICT: {
			res = strdup("d");
			for (size_t i = 0; i < b->dict.count; i++) {
				char* k = bec_encode(b->dict.keys[i]);
				char* v = bec_encode(b->dict.values[i]);
				size_t new_len = strlen(res) + strlen(k) + strlen(v) + 1;
				res = realloc(res, new_len + 1);
				strcat(res, k);
				strcat(res, v);
				free(k);
				free(v);
			}
			res = realloc(res, strlen(res) + 2);
			strcat(res, "e");
			break;
		}
	}
	return res;
}

void bec_clean(Bec *b) {
	free(b);
	return;
}
