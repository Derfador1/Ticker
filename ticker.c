
#include "ticker.h"

#include <string.h>

int main(int argc, char *argv[])
{
 //add code to test tree insert and stock create 
}

int compare_symbol(struct company *a, struct company *b)
{
	int value;
	value = strcmp(a->symbol, b->symbol); //the return value will be stored in value
	return value; //return to let program know is compare was successful
}

bool tree_insert(struct tree *t, struct company *comp, int (*cmp)(const struct company *, const struct company *))
{
	if(cmp(comp, t->data) < 1) {
		if(t->left) {
			return tree_insert(t->left, comp);//need to call cmp function
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


int cmp(const struct company *a, const struct company *b)
{
	if(a->cents < b->cents) {
		//less then
		return -1;
	}
	else if(a->cents > b->cents) {
		//greater then
		return 1;
	}
	else {
		//equal to
		return 0;
	}
}

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

market *market_create(void) 
{
	market *m = malloc(sizeof(*m));
	if(m) {
		m->root = NULL;
	}

	return m;
}

market *market_insert(market *m, struct company *comp, int (*cmp)(const struct company *, const struct company *))
{
	if(!m->root) {
		m->root = tree_create(comp);
	}
	else {
		tree_insert(m->root, comp, m->cmp);
	}
}

void market_destory(market *m) 
{

}
