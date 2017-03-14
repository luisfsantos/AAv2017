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

/* 
*  How to Compile this program 
*  gcc -O3 -ansi -Wall sma.c
*/


int content_size[2] = {0,0};
int space[2] = {1,1};
int buffer_size[2] = {1,1};
char *text;
char *pattern;

void test(void** func);
void run();
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

int main(int argc, char const *argv[])
{
	text = (char *) malloc(buffer_size[TEXT_IDX] * sizeof(char));
	pattern = (char *) malloc(buffer_size[PATTERN_IDX] * sizeof(char));

	if (argc > 0 && strncmp(TEST, argv[0], TEST_SIZE)) {
		/*test(run());*/
		/*run();*/
	}
	run();
	return 0;
}

void test(void ** func) {

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
	int pi_table[pattern_size];
	generate_pi_table(pi_table, pattern, pattern_size);
	/* debug */
	for (int i=0; i<pattern_size; i++) {
		printf("%d ", pi_table[i]);
	}
}

void generate_pi_table (int *pi_table, char * pattern, int pattern_size) {
	int i = 0;
	int pos = 1;
	pi_table[0] = 0; /* always like this */

	while (pos < pattern_size) {
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
}

void bm_algorithim(char * text, int text_size, char * pattern, int pattern_size) {
	int pos = 0;
	int i = pattern_size;
	while(pos <= text_size-pattern_size) {
		if (text[pos+i-1] == pattern[i-1] && i>0) {
			i--;
		} else {
			if (i == 0) {
				printf("%d\n", i);
			}
			i=pattern_size;
			pos = apply_skip(text, text_size, pos);
		}
	}
	printf("\n");
}

int apply_skip(char * text, int text_size, int pos) {
	return pos+1;
}