#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMMANDS
#define LINK 'L'
#define CUT 'C'
#define CONNECTED 'Q'
#define EXIT 'X'
#endif

#define INDEXOF(x) x-1

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

LCT allocLCT(int v);
void freeLCT(LCT t);
void access(LCT t, int v);
void reRoot(LCT t, int v);
int connectedQ(LCT t, int u, int v);
void link(LCT t, int r, int v);
void cut(LCT t, int r, int v);

void splay(LCT node);
void rotate_right(LCT node);
void rotate_left(LCT node);
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
	scanf("%d\n", &vertexes);
	tree = allocLCT(vertexes);
	do {
		command = getc(stdin);
		printTree(tree, vertexes);
	} while (execute_command(tree, command));
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
			scanf(" %d %d\n", &r, &v);
			link(t, r, v);
			break; 
		case CUT:
			/*TODO cut!!*/
			break;
		case CONNECTED:
			/*TODO connectedQ*/
			break;
		case EXIT:
			run = 0;
			/*TODO freeLTC(LCT t)*/
			break;
		default:
			printf("%s\n", "Not a command, please use the pattern (C \"space\" TEXT):");
			printf("\t%s\n", " - X to exit safely freeing all memory.");
			while (getc(stdin) != '\n');
			break;
	}
	return run;
}

void printTree(LCT t, int vertexes) {
	int i;
	for (i = 0; i<vertexes; i++) {
		printf("this: %p, left: %p, right: %p, hook: %p\n", (void*)&t[i], (void*)t[i].left, (void*)t[i].right, (void*)t[i].hook);
	}
}

LCT allocLCT(int v) {
	LCT tree = (LCT) malloc(v * sizeof(LCT));
	return tree;
}

void access(LCT t, int v) {
	LCT v_node = &t[INDEXOF(v)];

	splay(v_node);
	if (v_node->right != NULL) {
		v_node->right->hook = v_node;
		v_node->right = NULL;
	}
	while (!isRoot(v_node)) {
		LCT w = v_node->hook;
		splay(w);
		w->right = v_node;
		v_node->hook = w;
		splay(v_node);
	}
}

int isRoot(LCT node) {
	printf("%s\n", "in isRoot");
	if (node->hook == NULL) {
		return 1;
	}
	return node != node->hook->left && node != node->hook->right;
}

void splay(LCT node) {
	while (!isRoot(node)) {
		if( node->hook->hook == NULL ) {
			printf("%s\n", "node->hook->hook == NULL");
			if( node->hook->left == node ) rotate_right( node->hook );
			else rotate_left( node->hook );
		} else if( node->hook->left == node && node->hook->hook->left == node->hook ) {
			printf("%s\n", "node->hook->left == node && node->hook->hook->left == node->hook");
			rotate_right( node->hook->hook );
			rotate_right( node->hook );
		} else if( node->hook->right == node && node->hook->hook->right == node->hook ) {
			printf("%s\n", "node->hook->right == node && node->hook->hook->right == node->hook");
			rotate_left( node->hook->hook );
			rotate_left( node->hook );
		} else if( node->hook->left == node && node->hook->hook->right == node->hook ) {
			printf("%s\n", "node->hook->left == node && node->hook->hook->right == node->hook");
			rotate_right( node->hook );
			rotate_left( node->hook );
		} else {
			printf("%s\n", "else");
			rotate_left( node->hook );
			rotate_right( node->hook );
		}
	}
}

void rotate_right(LCT node) {
	printf("%s\n", "rotate_right");
	
	LCT node_left = node->left;

	if (node_left != NULL) {
		node->left = node_left->right;
		if (node_left->right != NULL) { node_left->right->hook = node; }
		node_left->hook = node->hook;
	}
	if (node->hook) {
		if ( node == node->hook->left ) node->hook->left = node_left;
    	else if (node == node->hook->right) node->hook->right = node_left;
	}

    if(node_left != NULL) node_left->right = node;
    node->hook = node_left;
    printf("%s\n", "end rotate_right");
}

void rotate_left(LCT node) {
	printf("%s\n", "rotate_left");
	LCT node_right = node->right;
	
	if (node_right != NULL) {
		node->right = node_right->left;
		if (node_right->left != NULL) { node_right->left->hook = node; }
		node_right->hook = node->hook;
	}
	if (node->hook) {
	    if ( node == node->hook->left ) node->hook->left = node_right;
	    else if (node == node->hook->right) node->hook->right = node_right;
	}

    if(node_right != NULL) node_right->left = node;
    node->hook = node_right;
    printf("%s\n", "end rotate_left");
}

LCT findRoot(LCT t, int r) {
	LCT node_r = &t[INDEXOF(r)];
	LCT left = node_r;
	printf("%s\n", "Before access");
	access(t, r);
	printf("%s\n", "Before while");
	while(left->left != NULL) {
		left = left->left;
	}
	printf("%s\n", "Before splay");
	splay(left);
	printf("%s\n", "After splay");
	return left;
}

void reRoot(LCT t, int v) {
	LCT node_v = &t[INDEXOF(v)];
	printf("%s\n", "Before findRoot");
	LCT represented_root = findRoot(t, v);
	printf("%s\n", "After ReRoot");
	node_v->left = represented_root;
	represented_root->hook = node_v;
}

void link(LCT t, int r, int v) {
	LCT node_r = &t[INDEXOF(r)];
	LCT node_v = &t[INDEXOF(v)];
	printf("%s\n", "Before ReRoot");
	reRoot(t, r);
	printf("%s\n", "Before access r");
	access(t, r);
	printf("%s\n", "Before access t");
	access(t, v);
	node_v->left = node_r;
	node_r->hook = node_v;
}