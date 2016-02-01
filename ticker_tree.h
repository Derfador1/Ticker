#ifndef TICKERTREE_H
	#define TICKERTREE_H

struct company {
	char symbol[6];
	size_t cents;
	char *name;
};

struct tree {
	struct company *data;
	struct tree *left, *right;
};

struct tree *tree_create(struct company *data);
void tree_destroy(struct tree *t);

#endif
