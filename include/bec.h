#ifndef BEC_H
#define BEC_H

typedef enum {
	BEC_INT,
	BEC_STRING,
	BEC_LIST,
	BEC_DIST,
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
			char **keys;
			BecType **values;
			long count;
		} dict;
	};
} Bec;

Bec* bec_parse(const char **input);
void bec_clean(Bec *b);
void bec_print(Bec *b);

#endif // BEC_H
