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

int size[2] = {1,1};
int space[2] = {1,1};
int location[2] = {0,0};
char *text;
char *pattern;

void test(void** func);
void run();
int update_buffer(char * buffer, int size);
int execute_command(char command);

int main(int argc, char const *argv[])
{
	text = (char *) malloc(sizeof(char));
	pattern = (char *) malloc(sizeof(char));

	if (argc > 0 && strncmp(TEST, argv[0], TEST_SIZE)) {
		/*test(run());*/
		run();
	}
	run();
	return 0;
}

void test(void ** func) {

}

/* 
* Function that runs the necessary code for commands to be processed 
*/
void run() {
	int size = 1;
	int space = 1;
	int location = 0;
	char *folder = (char *) malloc(size*sizeof(char));
	int carryOn = 1;
	do {

		char c;
		c = getc(stdin);

		if (c == '\n') {
			carryOn = 0;
			c = '\0';
		} else {
			carryOn = 1;
		}

		if(space > 0) {
			printf("I have space (%d) so I will place __ %c __\n", space, c);
			folder[location] = c;
			location++;
			space--;
		} else {
			space += size - 1;
			size = size * 2;
			printf("I have no space so I will double to %d and now I have %d free spaces and am placing __ %c __\n", size, space, c);
			folder = (char *) realloc(folder, size*sizeof(char));
			folder[location] = c;
			location++;
		}
	} while (carryOn);
	printf("%s\n", folder);
}

int execute_command(char command) {
	int run = 1;
	switch (command) {
		case TEXT_UPDATE:
			update_buffer(text, size[TEXT_IDX]);
			break; 
		case NAIVE:
			update_buffer(pattern, size[PATTERN_IDX]);
			/*UPDATE THE TEXT;*/
			break;
		case KMP:
			update_buffer(pattern, size[PATTERN_IDX]);
			/*UPDATE THE TEXT;*/
			break;
		case BM:
			update_buffer(pattern, size[PATTERN_IDX]);
			/*UPDATE THE TEXT;*/
			break;
		case EXIT:
			run = 0;
		default:
			printf("%s\n", "Not a command, RTFM!");
			break;
	}
	return run;
}


/* TODO decide if return size or make int size -> int* size*/
int update_buffer(char * buffer, int size) {
	int carryOn = 1;
	int space = size;
	int index = 0;
	do {

		char c;
		c = getc(stdin);

		if (c == '\n') {
			carryOn = 0;
			c = '\0';
		} /*else {
			carryOn = 1;
		}*/

		/*Grow and overwrite*/
		if(index < space) {
			printf("I have space (%d) so I will place __ %c __\n", space, c);
			buffer[index] = c;
			index++;
			space--;
		} else {
			space+=size-1;
			size = size*2;
			printf("I have no space so I will double to %d and now I have %d free spaces and am placing __ %c __\n", size, space, c);
			buffer = (char *) realloc(buffer, size*sizeof(char));
			buffer[index] = c;
			index++;
		}

		/*Delete*/
		if (index*4 < space) {
			space-=size/2-1;
			size = size/2;
			printf("I have too much space so I will half to %d and now I have %d free spaces \n", size, space);
			buffer = (char *) realloc(buffer, size*sizeof(char));
			/*buffer[index] = c;
			index++;*/
		}

	} while (carryOn);

	return size;
}