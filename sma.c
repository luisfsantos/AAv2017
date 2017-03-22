#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef TEST
#define TEST "test"
#define TEST_SIZE 4
#endif

#ifndef COMMANDS
#define TEXT_UPDATE 'T'
#define NAIVE 'N'
#define KMP 'K'
#define BM 'B'
#define EXIT 'X'
#endif

#ifndef INDEX
#define TEXT_IDX 0
#define PATTERN_IDX 1
#endif

#ifndef DYNAMIC_MEM
#define GROWTH 2
#define REDUCTION 4
#endif

#define ALPHABET_SIZE 4
/* 
*  How to Compile this program 
*  gcc -O3 -ansi -Wall sma.c
*/


int content_size[2] = {0,0};
int space[2] = {1,1};
int buffer_size[2] = {1,1};
char *text;
char *pattern;
int R[ALPHABET_SIZE];
char Alphabet[ALPHABET_SIZE] = {'A', 'C', 'G', 'T'};

void test(void** func);
void run();
void init();
char * update_buffer(char * buffer, int * size, int * content_size);
int execute_command(char command);
int calc_diff(int index, int space);
void delete_all();

/* ALGORITHIMS */
void naive_algorithim(char * text, int text_size, char * pattern, int pattern_size);

void kmp_algorithim(char * text, int text_size, char * pattern, int pattern_size);
void generate_pi_table (int *pi_table, char * pattern, int pattern_size);

void bm_algorithim(char * text, int text_size, char * pattern, int pattern_size);
int apply_skip(char * text, int text_size, int pos);
void z_preprocess(char * pattern, int pattern_size, int * Z);

int main(int argc, char const *argv[])
{
	text = (char *) malloc(buffer_size[TEXT_IDX] * sizeof(char));
	pattern = (char *) malloc(buffer_size[PATTERN_IDX] * sizeof(char));


	init();
	run();
	return 0;
}

void test(void ** func) {

}

void init() {
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
	char command;
	do {
		command = getc(stdin);
	} while (execute_command(command));
}



/*******************************************************************
* DESCRIPTION :     Executes the command that is given
*
* INPUTS :
*       PARAMETERS:
*           char      command               command to execute
*       GLOBALS :
*           char * 	  text 					array containing the text
*			char *    pattern 				array containing the pattern to match
* OUTPUTS :
*       PARAMETERS:
*           int    	  run             		whether the command was an exit or nor.
*       GLOBALS :
*            None
* PROCESS :
*
* NOTES :           
*/
int execute_command(char command) {
	int run = 1;
	switch (command) {
		case TEXT_UPDATE:
			text = update_buffer(text, &buffer_size[TEXT_IDX], &content_size[TEXT_IDX]);
			break; 
		case NAIVE:
			pattern = update_buffer(pattern, &buffer_size[PATTERN_IDX], &content_size[PATTERN_IDX]);
			naive_algorithim(text, content_size[TEXT_IDX], pattern, content_size[PATTERN_IDX]);
			break;
		case KMP:
			pattern = update_buffer(pattern, &buffer_size[PATTERN_IDX], &content_size[PATTERN_IDX]);
			kmp_algorithim(text, content_size[TEXT_IDX], pattern, content_size[PATTERN_IDX]);
			break;
		case BM:
			pattern = update_buffer(pattern, &buffer_size[PATTERN_IDX], &content_size[PATTERN_IDX]);
			bm_algorithim(text, content_size[TEXT_IDX], pattern, content_size[PATTERN_IDX]);
			break;
		case EXIT:
			run = 0;
			delete_all();
			break;
		default:
			printf("%s\n", "Not a command, please use the pattern (C \"space\" TEXT):");
			printf("\t%s\n", " - T to update the text.");
			printf("\t%s\n", " - N to run the pattern against the text with a naive implementation.");
			printf("\t%s\n", " - K to run the pattern against the text with a KMP implementation.");
			printf("\t%s\n", " - B to run the pattern against the text with a Boyer Moore implementation.");
			printf("\t%s\n", " - X to exit safely freeing all memory.");
			while (getc(stdin) != '\n');
			break;
	}
	return run;
}


/*******************************************************************
* DESCRIPTION :     Update a dynamic buffer with the DYNAMIC_MEM
					modifier (at 2 we have doubling)
*
* INPUTS :
*       PARAMETERS:
*           char*     buffer                pointer to the buffer
*			int*	  size 					pointer to the actual size of the buffer
*       GLOBALS :
*           int 	  GROWTH				dynamic memory growth on full
*			int 	  REDUCTION             dynamic memory reduction on lots of free space
* OUTPUTS :
*       PARAMETERS:
*           char    * temp_buff             new pointer to modified array
*       GLOBALS :
*            None
* PROCESS :
*                   [1]  get one char (should be a space " ")
*                   [2]  Do
*                   [3]    Get character
*					[4]	   if no space
*								apply dynamic memory growth
*					[5]	   place char 
*                   [6]  Until newline
*					[7]  if too much space
*							reduce size
*
* NOTES :           
*/
char * update_buffer(char * buffer, int * size, int * content_size) {
	int carryOn = 1;
	int space = *size;
	int index = 0;
	char * temp_buff = buffer;
	getc(stdin); /* ignore white space after the command */
	do {

		char c;
		c = getc(stdin);

		if (c == '\n') {
			carryOn = 0;
			c = '\0';
		} else {
			carryOn = 1;
		}

		/*Grow and overwrite*/
		if(space == 0) {
			space+=*size;
			*size = *size*GROWTH;
			printf("I have no space so I will double to %d and now I have %d free spaces \n", *size, space);
			temp_buff = (char *) realloc(buffer, *size*sizeof(char));
			if (temp_buff != NULL) {
				buffer = temp_buff;
			} else {
				exit(-1);
			}
		}
		printf("I have space (%d) so I will place __ %c __\n", space, c);
		buffer[index] = c;
		index++;
		space--;

		

	} while (carryOn);

	/*Delete*/
	if (index*REDUCTION < space) {
		*size = *size/calc_diff(index, space);
		space=*size-index-1;
		printf("I have too much space so I will reduce to %d and now I have %d free spaces \n", *size, space);
		temp_buff = (char *) realloc(buffer, *size*sizeof(char));
		if (temp_buff != NULL) {
			buffer = temp_buff;
		} else {
			exit(-1);
		}
	}

	*content_size = index-1;
	return temp_buff;
}


int calc_diff(int index, int space) {
	int diff = GROWTH;
	while (index*diff*REDUCTION < space) {
		diff = diff*GROWTH;
	}
	return diff;
}

void delete_all() {
	free(text);
	free(pattern);
}

void naive_algorithim(char * text, int text_size, char * pattern, int pattern_size) {
	int index = 0;
	int pattern_index = 0;
	int text_index = 0;
	int length = text_size-pattern_size;
	while (index <= length) {
		while(text[text_index] == pattern[pattern_index]) {
			if (pattern_index == pattern_size-1) {
				printf("%d ", index);
				break;
			}
			text_index++;
			pattern_index++;
		}
		index++;
		text_index = index;
		pattern_index = 0;
	}
	printf("\n");
}

void kmp_algorithim(char * text, int text_size, char * pattern, int pattern_size) {

	pattern_size++; /* ACCOUNT FOR \0*/
	int pi_table[pattern_size];
	generate_pi_table(pi_table, pattern, pattern_size);
	/* debug */
	for (int j=0; j<pattern_size; j++) {
		printf("%d ", pi_table[j]);
	}
	printf("\n");

	int comparisons = 0;
	int p = 0; /* the position of the current character in pattern */
	int t; /* the beginning of the current match in text */
	
	for (t = 0; t < text_size; t++) {

		while ((p > 0) && (pattern[p] != text[t])) {
			comparisons++;
			p = pi_table[p];
		}

		comparisons++;
		if (pattern[p] == text[t]) {
			p++;
		}

		if (p == (pattern_size-1)) {			
			printf("%d ", t - (pattern_size - 1) + 1); /* (pattern_size - 1) to not count \0 */
			/* p = pi_table[p-1];
			*  ^original algorithm has this line, but bc we're running a modified 
			*  version that includes the \0 in the pattern, this line doesn't make sense */
		}
	}

	printf("\n%d \n", comparisons);
}

void generate_pi_table (int *pi_table, char * pattern, int pattern_size) {
	int i = 0;
	int pos = 1;
	pi_table[0] = 0; /* always like this */

	while (pos < (pattern_size-1)) {
		if(pattern[pos] == pattern[i]) {
			i+=1;
			pi_table[pos]=i;
			pos+=1;
		} else if (i > 0) {
			i = pi_table[i];
		} else {
			pi_table[pos] = 0;
			pos+=1;
		}
	}
	/* entry for \0 always the same as entry for last actual pattern char */
	pi_table[pos] = pi_table[pos-1];
}

void bm_algorithim(char * text, int text_size, char * pattern, int pattern_size) {
	int pos = 0;
	int i = pattern_size;
	int comparisons = 0;
	int Z[pattern_size];
	z_preprocess(pattern, pattern_size, Z);
	while(pos <= text_size-pattern_size) {
		comparisons++;
		if (text[pos+i-1] == pattern[i-1] && i>0) {
			i--;
		} else {
			if (i == 0) {
				printf("%d ", pos);
			}
			i=pattern_size;
			pos = apply_skip(text, text_size, pos);
		}
	}
	printf("\n");
	printf("%d \n", comparisons);
}

int apply_skip(char * text, int text_size, int pos) {
	int bc = 1/*= bad_character()*/;
	int sgs = 0/*= strong_good_suffix()*/;
	return (bc > sgs) ? bc: sgs;
}

void z_preprocess(char * pattern, int pattern_size, int * Z) {
	int l = 0;
	int r = 0;
	int k;
	Z[0] = 0; /* Zi for all 0 < i < k-1 */
	for (k = 1; k < pattern_size; k++) { /*because we want a proper prefix i=1*/
		if(k>r) {
			/* if k was larger than r in this iteration it will be larger in the next if Zk=0
			* this means the following r=k has no effect on the algorithim */
			r = k; 
			while (pattern[r-k]==pattern[r] && r<pattern_size) {
				r++;
			}
			Z[k] = r-k; /* r - k in this case is equivalent to the length */
			if ((r-k) > 0) { 
				r--; /* we only do r-- as what we want is r=k+Zk-1 but Zk is r-k so r = k+r-k-1 <->  r=r-1*/
				l = k;
			} 
		} else {
			int k_prime = k - l;
			if (Z[k_prime] < (r-k)) {
				Z[k] = Z[k_prime];
			} else {
				while (pattern[r-k]==pattern[r] && r<pattern_size) {
					r++;
				}
				Z[k] = r-k;
				l=k;
				r--;
			}
		}
	}
}

int bad_character(int * R, char check, int index) {
	return 1;
}