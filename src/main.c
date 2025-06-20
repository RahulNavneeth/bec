#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int* items;
	size_t size;
	size_t cap;
} Num;

#define da_append(x, i)                             \
    do {                                            \
        if (x.size >= x.cap) {                      \
            if (x.cap == 0) x.cap = 2;              \
            else x.cap *= 2;                        \
            x.items = realloc(x.items,              \
                                x.cap * sizeof(*x.items)); \
        }                                           \
        x.items[x.size++] = i;                      \
    } while (0)

int32_t main() {
	Num x = {0};
	for (int i = 0 ; i < 10 ; i++) {
		da_append(x, i+1);
	}
	for (size_t i = 0 ; i < 6 ; i++) {
		printf("%d\n", x.items[i]);
	}
	return 0;
}
