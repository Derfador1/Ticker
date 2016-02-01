
#include "ticker_tree.h"

struct tree *tree_create(struct company *data)
{
	struct tree *t = malloc(sizeof(*t));
	if(t) {
		t->data = data;
		t->left = NULL;
		t->right = NULL;
	}

	return t;
}

void tree_destroy(struct tree *t)
{
	if(!t) {
		return;
	}

	tree_destroy(t->left);
	tree_destroy(t->right);

	free(t);
}

