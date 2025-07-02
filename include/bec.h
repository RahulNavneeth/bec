#ifndef BEC_H
#define BEC_H

typedef enum {
	BEC_INT,
	BEC_STING,
	BEC_LIST,
	BEC_DIST,
} BecType;

typedef struct {
	BecType type;
	union {
		long integer;
		struct {
			char *str;
			int len;
		} string;
		struct {
			BecType **items;
			int count;
		} list;
		struct {
			char **keys;
			BecType **values;
			int count;
		} dict;
	};
} Bec;

Bec* bec_parse(const char **input);
void bec_clean(Bec *b);
void bec_print(Bec *b);

char bec_next(const char **input);
char bec_get(const char **input);

#endif // BEC_H
