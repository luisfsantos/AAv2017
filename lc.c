/*
* Catarina Cepeda 77966
* Luis Santos 77900
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMMANDS
#define LINK 'L'
#define CUT 'C'
#define CONNECTED 'Q'
#define ACCESS 'A'
#define EXIT 'X'
#endif

#define INDEXOF(x) x-1

#define VERBOSE 0

typedef struct LCT *LCT;

struct LCT { /** Represents a node in the LCT. */
  LCT left; /** Child */
  LCT right; /** Child */
  LCT hook; /** General Parent Pointer. */
  int sum; /** The size of this sub-tree.
  Negative values are used to indicate that
left and right pointers should be swapped
on the sub-tree. */
};


void run();
int execute_command(LCT t, char command);

void printTree(LCT t, int vertexes);
int findNumber(LCT t, LCT node, int vertexes);

LCT allocLCT(int v);
void freeLCT(LCT t, int vertexes);
void access(LCT t, int v);
void reRoot(LCT t, int v);
LCT findRoot(LCT t, int r);
int connectedQ(LCT t, int u, int v);
void link(LCT t, int r, int v);
void cut(LCT t, int r, int v);
int edgeConnected(LCT t, int u, int v);

void splay(LCT node);
void rotate_right(LCT node);
void rotate_left(LCT node);
void normalize(LCT node);
void flip(LCT t, int u);
int isRoot(LCT node);


int main(int argc, char const *argv[])
{
	run();
	return 0;
}

/* ================================================================
* Read a char from input and process the command 
* (assumes this is infact a new line)
*==================================================================
* This function is the entry point into the application and
* due to the way the buffer grows it does not read a whole line.
* The task of executing a command includes reading the necessary
* chars from the input.
*==================================================================
*/
void run() {
	int vertexes;
	char command;
	LCT tree;
	scanf("%d", &vertexes);
	tree = allocLCT(vertexes);
	do {
		getc(stdin); /*remove new line*/
		command = getc(stdin);
		if (VERBOSE) printTree(tree, vertexes);
	} while (execute_command(tree, command));
	freeLCT(tree, vertexes);
}



/*******************************************************************
* DESCRIPTION :     Executes the command that is given
*
* INPUTS :
*       PARAMETERS:
*           char      command               command to execute
* OUTPUTS :
*       PARAMETERS:
*           int    	  run             		whether the command was an exit or nor.
*       GLOBALS :
*            None
* PROCESS :
*
* NOTES :           
*/
int execute_command(LCT t, char command) {
	int run = 1;
	int v;
	int u;
	int r;
	switch (command) {
		case LINK:
			scanf(" %d %d", &r, &v);
			link(t, r, v);
			break; 
		case CUT:
			scanf(" %d %d", &r, &v);
			cut(t, r, v);
			break;
		case CONNECTED:
			scanf(" %d %d", &u, &v);
			if (connectedQ(t, u, v)) {
				printf("T\n");
			} else {
				printf("F\n");
			}
			break;
		case ACCESS:
			scanf(" %d", &u);
			access(t, u);
			break;
		default:
			run = 0;
			break;
	}
	return run;
}

void printTree(LCT t, int vertexes) {
	int i;
	printf("%s\n", "-----TREE-----");
	for (i = 0; i<vertexes; i++) {
		printf("%d, left: %d, right: %d, hook: %d, sum: %d\n", findNumber(t, &t[i], vertexes), findNumber(t, t[i].left, vertexes), findNumber(t, t[i].right, vertexes), findNumber(t, t[i].hook, vertexes), t[i].sum);
	}
	printf("%s\n", "-----TREE-----");
}

int findNumber(LCT t, LCT node, int vertexes) {
	int i;
	for (i = 0; i<vertexes; i++) {
		if (&t[i] == node) return i+1;
	}
	return 0;
}

LCT allocLCT(int v) {
	LCT tree = malloc(v * sizeof(struct LCT));
	int i = 0;
	for (i = 0; i < v; i++)
	{
		tree[i].right = NULL;
		tree[i].left = NULL;
		tree[i].hook = NULL;
		tree[i].sum = 0;
	}
	return tree;
}

void freeLCT(LCT t, int vertexes) {
	free(t);
}

void normalize(LCT node) {
	/*printf("%s\n", "@ Normalize");*/
	if (node->sum) {
		node->sum = !node->sum;
		LCT aux_left = node->left;
		LCT aux_right = node->right;
		node->left = aux_right;
		node->right = aux_left;
		if (aux_right) {
			aux_right->sum = !aux_right->sum;
		}
		if (aux_left) {
			aux_left->sum = !aux_left->sum;
		}
		
	}
}

void flip(LCT t, int u) {
	/*printf("%s\n", "@ Flip");*/
	LCT node = &t[INDEXOF(u)];
	node->sum = !node->sum;
}

void access(LCT t, int v) {
	/*printf("%s\n", "@ Access");*/
	LCT v_node = &t[INDEXOF(v)];
	LCT v_copy = v_node;
	splay(v_node);
	if (v_node->right != NULL) {
		v_node->right->hook = v_node;
		v_node->right = NULL;
	}
	while (v_copy->hook != NULL) {
		LCT w = v_copy->hook;
		splay(w);

		/* set pathparent of w right to w*/
		if (w->right) w->right->hook = w;

		/*set v to w right*/
		w->right = v_copy;
		w->right->hook = w;

		v_copy = w;
	}
	splay(v_node);
}

int isRoot(LCT node) {
	/*printf("%s\n", "@ isRoot");*/
	return node->hook == NULL || (node != node->hook->left && node != node->hook->right);
}

void splay(LCT node) {
	/*printf("%s\n", "@ Splay");*/
	while (!isRoot(node)) {
		LCT parent = node->hook;
		if(isRoot(parent)) {
			normalize(parent);
			normalize(node);
			if( parent->left == node ) {
				rotate_right( node );
			}
			else {
				rotate_left( node );
			}
		} else {
			LCT grandparent = parent->hook;
			if(!isRoot(grandparent)) { normalize(grandparent->hook); } /*sometimes the grandparent of the parent is accessed in the rotations*/
			normalize(grandparent);
			normalize(parent);
			normalize(node);
			if (grandparent->left == parent) {
				if (parent->left == node) {
					rotate_right(parent);
					rotate_right(node);
				} else {
					rotate_left(node);
					rotate_right(node);
				}
			} else {
				if (parent->right == node) {
					rotate_left(parent);
					rotate_left(node);
				} else {
					rotate_right(node);
					rotate_left(node);
				}
			}
		}
	}
	normalize(node);
}

void rotate_right(LCT node) {
	/*printf("%s\n", "@ RoR");*/
	/*we are sure parent exists because these are aux funcions of splay*/
	LCT parent = node->hook;
	LCT grandparent = parent->hook;
	parent->left = node->right;
	if (node->right != NULL) {
		parent->left->hook = parent;
	}
	node->right = parent;
	parent->hook = node;
	node->hook = grandparent;
	if (grandparent != NULL) {
		if (grandparent->left == parent) {
			grandparent->left = node;
		} else if (grandparent->right == parent) {
			grandparent->right = node;
		}
	}
}

void rotate_left(LCT node) {
	/*printf("%s\n", "@ RoL");*/
	/*we are sure parent exists because these are aux funcions of splay*/
	LCT parent = node->hook;
	LCT grandparent = parent->hook;
	parent->right = node->left;
	if (node->left != NULL) {
		parent->right->hook = parent;
	}
	node->left = parent;
	parent->hook = node;
	node->hook = grandparent;
	if (grandparent != NULL) {
		if (grandparent->left == parent) {
			grandparent->left = node;
		} else if (grandparent->right == parent) {
			grandparent->right = node;
		}
	}
}

LCT findRoot(LCT t, int r) {
	LCT node_r = &t[INDEXOF(r)];
	LCT left = node_r;
 	access(t, r);
 	while(left->left != NULL) {
 		left = left->left;
 		normalize(left);
 	}
 	return left;
 }

int edgeConnected(LCT t, int u, int v) {
	if (u == v) {
		return 0;
	}
	LCT node_u = &t[INDEXOF(u)];
	LCT node_v = &t[INDEXOF(v)];
	reRoot(t, u);
	access(t, u);
	access(t, v);
	return node_u == node_v->left && !node_u->right;
}

void reRoot(LCT t, int v) {
	access(t, v);
	flip(t, v);
}

int connectedQ(LCT t, int u, int v) {
	if (u == v) {
		return 1;
	}
	LCT node_u = &t[INDEXOF(u)];
	reRoot(t, u);
	access(t, u);
	return node_u == findRoot(t, v);
}

void cut(LCT t, int r, int v) {
	LCT node_v = &t[INDEXOF(v)];
	if (edgeConnected(t, r, v)) {
		/*printf("C %d %d\n", r, v);*/
		/*reRoot(t, v); not needed as connectedQ reroots to v */
		/*access(t, r); not needed as connectedQ accesses r last*/
		/*normalize(node_r); not needed as connectedQ has an access to r which should normalize r*/
		if(node_v->left) node_v->left->hook = NULL;
		node_v->left = NULL;
	}
}

void link(LCT t, int r, int v) {
	LCT node_r = &t[INDEXOF(r)];
	LCT node_v = &t[INDEXOF(v)];
	if (!connectedQ(t, r, v)) {
		/*printf("L %d %d\n", r, v);*/
		/*reRoot(t, r); not needed as connectedQ reroots to r*/
		/*access(t, r); not needed as connectedQ accesses r and v in this order*/
		/*access(t, v); explained above*/
		node_r->left = node_v;
		node_v->hook = node_r;
	}
}