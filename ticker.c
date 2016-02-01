
#include <string.h>

#include "ticker.h"

int main(int argc, char *argv[])
{
	FILE *fp;

	if(argc != 2) {
		printf("Error\n");
		return 0;
	}
	else {
		fp = fopen(argv[1], "r");
		if(!fp) {
			printf("suck it\n");
			return 0;
		}
	}

	char symbol[6];
	double cents;
	char name[65];

	market *m = market_create();

	while(feof(fp)) {
		fscanf(fp, "%s, %lf, %[^\n]", symbol, &cents, name);
		struct company *comp = stock_create(symbol, name, cents);
		market_insert(m, comp, compare_symbol);
	}

	tree_inorder(m->root);

	fclose(fp);
	return 0;
}

int compare_symbol(const struct company *a, const struct company *b)
{	
	int value = strcmp(a->symbol, b->symbol); //the return value will be stored in value
	return value; //return to let program know is compare was successful
}

bool tree_insert(struct tree *t, struct company *comp, int (*cmp)(const struct company *a, const struct company *b))
{
	if(cmp(comp, t->data) < 1) {
		if(t->left) {
			return tree_insert(t->left, comp, cmp);//need to call cmp function
		}
		else {
			t->left = tree_create(comp);
			return t->left;
		}
	}
	else {
		if(t->right) {
			return tree_insert(t->right, comp, cmp);
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

market *market_insert(market *m, struct company *comp, int (*cmp)(const struct company *a, const struct company *b))
{
	if(!m->root) {
		m->root = tree_create(comp);
	}
	else {
		tree_insert(m->root, comp, cmp);
	}

	return m;
}

void market_destory(market *m) 
{

}

void tree_inorder(struct tree *t)
{
	if(!t) {
		return;
	}

	tree_inorder(t->left);
	printf("%s %zd %s\n", t->data->symbol, t->data->cents, t->data->name);
	tree_inorder(t->right);
}

