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

int size[2] = {1,1};
int space[2] = {1,1};
int location[2] = {0,0};
char *text;
char *pattern;

void test(void** func);
void run();
char * update_buffer(char * buffer, int * size);
int execute_command(char command);
int calc_diff(int index, int space);
void delete_all();

int main(int argc, char const *argv[])
{
	text = (char *) malloc(size[TEXT_IDX] * sizeof(char));
	pattern = (char *) malloc(size[PATTERN_IDX] * sizeof(char));

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
		printf("%s\n", text);
		//printf("%c\n", command);
		//printf("%c\n", getc(stdin));/* ignore white space after the command */
	} while (execute_command(command));
}

int execute_command(char command) {
	int run = 1;
	switch (command) {
		case TEXT_UPDATE:
			text = update_buffer(text, &size[TEXT_IDX]);
			break; 
		case NAIVE:
			pattern = update_buffer(pattern, &size[PATTERN_IDX]);
			/*UPDATE THE TEXT;*/
			break;
		case KMP:
			pattern = update_buffer(pattern, &size[PATTERN_IDX]);
			/*UPDATE THE TEXT;*/
			break;
		case BM:
			pattern = update_buffer(pattern, &size[PATTERN_IDX]);
			/*UPDATE THE TEXT;*/
			break;
		case EXIT:
			run = 0;
			delete_all();
			break;
		default:
			printf("%s\n", "Not a command, RTFM!");
			break;
	}
	return run;
}


/* TODO decide if return size or make int size -> int* size*/
char * update_buffer(char * buffer, int * size) {
	int carryOn = 1;
	int space = *size;
	int index = 0;
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
		if(space > 0) {
			printf("I have space (%d) so I will place __ %c __\n", space, c);
			buffer[index] = c;
			index++;
			space--;
		} else {
			space+=*size-1;
			*size = *size*GROWTH;
			printf("I have no space so I will double to %d and now I have %d free spaces and am placing __ %c __\n", *size, space, c);
			buffer = (char *) realloc(buffer, *size*sizeof(char));
			buffer[index] = c;
			index++;
		}

		

	} while (carryOn);

	/*Delete*/
	if (index*REDUCTION < space) {
		*size = *size/calc_diff(index, space);
		space=*size-index-1;
		printf("I have too much space so I will reduce to %d and now I have %d free spaces \n", *size, space);
		buffer = (char *) realloc(buffer, *size*sizeof(char));
		/*buffer[index] = c;
		index++;*/
	}

	//printf("%d\n", *size);
	return buffer;
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