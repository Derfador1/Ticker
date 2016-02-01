
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

	market *m = market_create(compare_symbol);

	while(!feof(fp)) {
		fscanf(fp, "%s %lf",  symbol, &cents);
		printf("%s\n", symbol);
		if(fgetc(fp) == ' ') {
			fscanf(fp, "%[^\n]", name);
		}
		else if(fgetc(fp) == EOF) {
			break;
		}
		struct company *comp = stock_create(symbol, name, cents);
		market_insert(m, comp);
	}

	tree_inorder(m->root);

	char operator;
	char symbol2[6];
	double cents2;
	char name2[65];
	while(!feof(stdin)) {
		fscanf(stdin, "%s %c%lf", symbol2, &operator, &cents2);
		printf("%s\n", symbol);
		struct company *comp1 = stock_create(symbol2, name2, cents);
		market_insert(m, comp1);
	}

	tree_inorder(m->root);



	fclose(fp);
	//market_destroy(m);
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

market *market_create(int (*cmp)(const struct company *a, const struct company *b))
{
	market *m = malloc(sizeof(*m));
	if(m) {
		m->root = NULL;
		m->cmp = cmp;
	}

	return m;
}

market *market_insert(market *m, struct company *comp)
{
	if(!m->root) {
		printf("tree create\n");
		m->root = tree_create(comp);
	}
	else if(m->cmp(comp, m->root->data) == 0) {
		printf("tree add\n");
		m->root->data->cents += comp->cents;
	}
	else {
		printf("tree insert\n");
		tree_insert(m->root, comp, m->cmp);
	}

	return m;
}


void market_destory(market *m) 
{
	if(!m) {
		return;
	}	

	tree_destroy(m->root);
	free(m);
}


void tree_inorder(struct tree *t) //change to market inorder
{
	if(!t) {
		return;
	}

	tree_inorder(t->left);
	printf("%s %zd.%zd %s\n", t->data->symbol, t->data->cents / 100, t->data->cents % 100, t->data->name);
	tree_inorder(t->right);
}
