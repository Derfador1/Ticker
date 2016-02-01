#ifndef TICKER_H
	#define TICKER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


struct company {
	char symbol[6];
	ssize_t cents;
	char *name;
};

struct tree {
	struct company *data;
	struct tree *left, *right;
};


typedef struct {
	struct tree *root;
	int (*cmp)(const struct company *, const struct company *);
} market;

bool tree_insert(struct tree *t, struct company *comp, int (*cmp)(const struct company *a, const struct company *b));
struct company *stock_create(char *symbol, char *name, double price);
int compare_symbol(const struct company *a, const struct company *b);
int compare_cents(const struct company *a, const struct company *b);
struct tree *tree_create(struct company *data);
void tree_destroy(struct tree *t);
market *market_create(int (*cmp)(const struct company *a, const struct company *b));
market *market_insert(market *m, struct company *comp); 
void market_destory(market *m);
void tree_inorder(struct tree *t);
void tree_copy(struct tree *dst, struct tree *src, int (*cmp)(const struct company *a, const struct company *b)

#endif
