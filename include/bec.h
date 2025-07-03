#ifndef BEC_H
#define BEC_H

typedef enum {
	BEC_INT,
	BEC_STRING,
	BEC_LIST,
	BEC_DICT,
} BecType;

typedef struct {
	BecType type;
	union {
		long integer;
		struct {
			char *str;
			long len;
		} string;
		struct {
			BecType **items;
			long count;
		} list;
		struct {
			BecType **keys;
			BecType **values;
			long count;
		} dict;
	};
} Bec;

Bec* bec_decode(const char **input);
char* bec_encode(Bec *b);
void bec_clean(Bec *b);
void bec_print(Bec *b, int);

#endif // BEC_H
