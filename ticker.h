#ifndef TICKER_H
	#define TICKER_H

#include <stdbool.h>
#include <stdlib.h>
#include "ticker_tree.h"

/*
struct company {
	char symbol[6];
	size_t cents;
	char *name;
};

struct tree {
	struct company *data;
	struct tree *left, *right;
};
*/

typedef struct {
	struct tree *root;
	int (*cmp)(const struct company *, const struct company *);
} market;

bool tree_insert(struct tree *t, struct company *comp, int (*cmp)(const struct company *, const struct company *));
struct company *stock_create(char *symbol, char *name, double price);
int compare_name(struct company *a, struct company *b);

