
#include <string.h>

#include "ticker.h"

int main(int argc, char *argv[])
{
	FILE *fp;
	//check validity of command line
	if(argc != 2) {
		printf("Error\n");
		return 0;
	}
	else {
		fp = fopen(argv[1], "r");
		if(!fp) { //if the file we are trying to open doesnt exist
			printf("suck it\n");
			return 0;
		}
	}

	char symbol[6];
	double cents;
	char name[65];

	//makes my market m and dst_m
	market *m = market_create(compare_symbol); 
	market *dst_m = market_create(compare_cents);

	struct company *comp1;
	struct company *comp;

	char symbol2[6];
	char name2[65];
	int ch;

	name2[0] = '\0'; //makes sure name 2 is just a null byte

	while(!feof(fp)) {
		name[0] = '\0';
		if (2 != fscanf(fp, "%5s %lf",  symbol, &cents)) { //if the fscanf doesnt return 2 things matched we will break
			if(feof(fp)) {
				break;
			}
			else {
				fprintf(stderr, "There is something wrong with your companies, exiting\n");
				fclose(fp);
				market_destroy(m);
				market_disassembler(dst_m);
				exit(1);
			}
		}

		if(fgetc(fp) == ' ') {
			fscanf(fp, "%[^\n]", name); //if there is still a space after our fscanf we pull in the string as name
		}
		else if(fgetc(fp) == EOF) { //otherwise we take in the rest of the fp
			break;
		}
		comp = stock_create(symbol, name, cents); 
		market_upsert(m, comp); //call market upsert with inserts and updates
	}

	while(!feof(stdin)) {
		double cents2 = 0;
		if(2 != fscanf(stdin, "%5s %lf", symbol2, &cents2)) { //makes sure whats input into fscanf s is 5 characters long
			if(feof(stdin)) { //if we have no input we break
				printf("^D\n");
				break;
			}
			else {
				printf("Error invalid input\n");
				while((ch = getchar()) != '\n' && ch != EOF) {
					//get rid of rest of standard input
				}
				continue;
			}

		}

		comp1 = stock_create(symbol2, name2, cents2);

		market_upsert(m, comp1);

		stock_destroyer(comp1);
	}

	market_copy(dst_m, m->root); //copies the tree to the dst market

	market_inorder(dst_m); //printf the tree

	fclose(fp);

	market_destroy(m);
	market_disassembler(dst_m);
	
	return 0;
}

bool tree_insert(struct tree *t, struct company *comp, int (*cmp)(const struct company *a, const struct company *b))
{
	ssize_t temp = t->data->cents + comp->cents; //used to check against .01 value
	//liams code for the tree insert file
	if(cmp(comp, t->data) < 0) {
		if(t->left) {
			return tree_insert(t->left, comp, cmp);
		}
		else {
			t->left = tree_create(comp);
			return t->left;
		}
	}
	else if(cmp(comp, t->data) == 0) {
		if(temp >= .01) { //if the temp is >= .01 then add the new comp cents and return false
			t->data->cents += comp->cents;
			return true;
		}
		else
		{
			fprintf(stderr, "Not valid input\n"); //otherwise destroy the stock
			return false;
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
	//liams code that creates new company
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

int compare_symbol(const struct company *a, const struct company *b)
{
	int value = strcmp(a->symbol, b->symbol); //the return value will be stored in value
	return value; //return to let program know is compare was successful
}

int compare_cents(const struct company *a, const struct company *b)
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

//creates a tree node from the company with left and right set to null
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

//creates a market with a null root and a compare function
market *market_create(int (*cmp)(const struct company *a, const struct company *b))
{
	market *m = malloc(sizeof(*m));
	if(m) {
		m->root = NULL;
		m->cmp = cmp;
	}

	return m;
}

//updates and inserts market
market *market_upsert(market *m, struct company *comp)
{
	if(!m->root) {
		m->root = tree_create(comp);
	}
	else {
		if(!tree_insert(m->root, comp, m->cmp))
		{
			stock_destroyer(comp); //if we got false back the we destroy the comp
		}
	}

	return m;
}

//copies to dst_m from the tree
void market_copy(market *dst_m, struct tree *t)
{
	if(!t) {
		return;
	}

	if(!dst_m) {
		dst_m = market_create(compare_cents);
	}


	market_upsert(dst_m, t->data);
	market_copy(dst_m, t->left); 
	market_copy(dst_m, t->right); 
}

//calls tree inorder and prints the market out
void market_inorder(market *m)
{
	if(!m) {
		return;
	}

	tree_inorder(m->root);
}

//prints out the tree data in order
void tree_inorder(struct tree *t) //change to market inorder
{
	if(!t) {
		return;
	}

	tree_inorder(t->left);
	printf("%s %zd.%zd %s\n", t->data->symbol, t->data->cents / 100, t->data->cents % 100, t->data->name);
	tree_inorder(t->right);
}

//calls tree destroy and free the market so we can free everything
void market_destroy(market *m) 
{
	if(!m) {
		return;
	}	


	tree_destroy(m->root);
	free(m);
}

//calls tree disassemble and frees the market to free the structure
void market_disassembler(market *m)
{
	if(!m) {
		return;
	}

	tree_disassembler(m->root);
	free(m);
}

//destroy the tree by recursively calling itself
void tree_destroy(struct tree *t)
{
	if(!t) {
		return;
	}

	
	tree_destroy(t->left); //recursion
	tree_destroy(t->right); //recursion
	stock_destroyer(t->data);
	free(t);
}

//disassembles the tree and sets the nodes to null
void tree_disassembler(struct tree *t)
{
	if(!t) {
		return;
	} 

	tree_disassembler(t->left); //recursion
	t->left = NULL;
	tree_disassembler(t->right); //recursion
	t->right = NULL;
	free(t);
}

//used to destroy the stock/company that was created
void stock_destroyer(struct company *c)
{
	if (!c) {
		return;
	}

	free(c->name); //free the companys data
	free(c); //free the company
}
