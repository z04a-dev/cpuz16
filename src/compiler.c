/* This is test-field for creating a compiler for CPUZ16 */
/* by z04a */

/* This is not working now! Currently in the process of refactoring lexer logic */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* I've implemented check for byte array size, but please -> do not change it!*/
/* But it also may change it the future, so keeping it for now */

#define NUM_CELLS 32768 /* there is 32768 cells of 16 bit values in 64 KiB */
#define MAGIC_SIZE 1 /* 1 magic cell would be enough */
/* there is no support for MAGIC sizes > 1 */

#define TOTAL_CELLS NUM_CELLS+MAGIC_SIZE

#define MAX_ROM_SIZE 65536 /* CPUZ16 can't load more than 64 KiB of memory */

#define MAGIC_VALUE 0xFF3A

#define BYTECODE_START MAGIC_SIZE /* code will start at array[MAGIC_SIZE], which will put it right after magic */

#define SWAP_ENDIANNESS false 

#define READ_BINARY true 

/* Final size of 32768 + 1 array must be 65538 */

typedef unsigned short u16;

bool check_array_size(unsigned int byte_array_size) {
	if (byte_array_size - (MAGIC_SIZE * sizeof(u16)) > MAX_ROM_SIZE) {
		printf("PANIC: Size of byte array exceeds %d, that's not valid!\n", MAX_ROM_SIZE);
		printf("PANIC: Size of byte array is %d, which is greater than MAX_ROM_SIZE: %d\n", 
				(int)(byte_array_size - (MAGIC_SIZE * sizeof(u16))), MAX_ROM_SIZE);
		printf("PANIC: Did you change NUM_CELLS to something huge?\n");
		return false;
	} else {
		return true;
	}
} 

void byte_array_magic(u16 *array) {
	printf("Putting magic into array...\n");
	array[0] = MAGIC_VALUE;
}

u16 *create_byte_array() {
	unsigned int byte_array_size = (TOTAL_CELLS) * sizeof(u16);
	if (!check_array_size(byte_array_size)) {
		printf("PANIC: Failed to allocate byte array!\n");
		exit(1);
	}
	printf("Allocating byte array<%d+%d>...\n", (int)(MAGIC_SIZE * sizeof(u16)), (int)(byte_array_size - (MAGIC_SIZE * sizeof(u16))));

	/* Using calloc, so array gets initialied with 0 */
	// u16 *array = malloc(byte_array_size);
	u16 *array = calloc(TOTAL_CELLS, sizeof(u16));
	if (array == NULL) {
		printf("PANIC: Failed to allocate bytecode\n");
		printf("PANIC: Are you out of memory? calloc() failed!\n");
		exit(1);
	}
	if (!READ_BINARY)
		byte_array_magic(array);
	return array;
}

void destroy_byte_array(u16 *array) {
	printf("Destroying byte array...\n");
	// TODO
	// Check if array was actually allocated, or it will segfault!
	free(array);
}


void print_byte_array(u16 *array, u16 start, u16 end) {
	u16 *p = &array[start];
	for (int count = start; p < &array[end]; ++p, ++count) {
		if (count == 0 || count % 4 == 0)
			printf("\n%05d ... %05d: ", count+1, count + 4);
		printf("0x%04X ", *p);
	}
	printf("\n");
}

bool create_binary(u16 *array) {
	char *filename = "output.bin";
	FILE *fp;
	fp = fopen(filename, "wb+");


	u16 *p = array;
	for (; p < &array[TOTAL_CELLS]; ++p) {
		if (SWAP_ENDIANNESS) {
			char low_byte = *p & 0xFF;
			char high_byte = (*p & 0xFF00) >> 8;
			fwrite(&high_byte, sizeof(char), 1, fp);
			fwrite(&low_byte, sizeof(char), 1, fp);
		} else {
			fwrite(p, sizeof(u16), 1, fp);
		}
	}
	fclose(fp);
	return true;
}

void read_binary() {
	char *filename = "output.bin";
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("PANIC: Failed to open <%s>...\n", filename);
		exit(1);
	}
	u16 *array = create_byte_array();
	u16 *p = array;
	for (; p < &array[TOTAL_CELLS]; ++p) {
		if (fread(p, sizeof(u16), 1, fp) != 1)
			break;
	}
	fclose(fp);
	print_byte_array(array, 0, TOTAL_CELLS);
	destroy_byte_array(array);
}

int main(void) {

	printf("ERROR: COMPILER IS NOT FINISHED. EXITING...\n");
	return(1);

	if (READ_BINARY) {
		read_binary();
		return 0;
	}

	u16 *bytecode = create_byte_array();

	/* REMOVE: This is just for testing binary */

	bytecode[BYTECODE_START] = 0x1717;

	/* REMOVE to here */

	print_byte_array(bytecode, 0, 6);

	printf("Creating binary...\n");
	create_binary(bytecode);
	goto clean;

clean:
	destroy_byte_array(bytecode);
	return 0;
}
