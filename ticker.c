
#include "ticker.h"

#include <string.h>

bool tree_insert(struct tree *t, struct company *comp, int (*cmp)(const struct company *, const struct company *))
{
	if(cmp(comp, t->data) < 1) {
		if(t->left) {
			return tree_insert(t->left, comp);
		}
		else {
			t->left = tree_create(comp);
			return t->left;
		}
	}
	else {
		if(t->right) {
			return tree_insert(t->right, comp);
		}
		else {
			t->right = tree_create(comp);
			return t->right;
		}
	}
}

struct company *stock_create(char *symbol, char *name, double price)
{
	struct company *new_stock = malloc(sizeof(*new_stock));
	if(!new_stock) {
		return NULL;
	}

	new_stock->name = strdup(name);
	if(!new_stock->name) {
		free(new_stock);
		return NULL;
	}

	strncpy(new_stock->symbol, symbol, sizeof(new_stock->symbol)-1);
	new_stock->symbol[sizeof(new_stock->symbol)-1] = '\0';
	
	new_stock->cents = 100 * price;

	return new_stock;
}