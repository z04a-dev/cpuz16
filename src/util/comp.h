#ifndef _UTIL_COMPILER
#define _UTIL_COMPILER

#ifndef _UTIL_COMPILER_VALUES
#include "comp-values.h"
#endif

#include "stdio.h"

#ifndef _STRUCT_IMPL
#include "../struct.h"
#endif


#include <stdlib.h>
#include <stdio.h>


#define PRINT_AS_BINARY true 
#define READ_BINARY false

// TODO
// fix when it prints more than 6 values in a row
void print_byte_array(u16 *array, u16 start, u16 end) {
	u16 *p = &array[start];
	u16 nil_count = 0;
	for (int count = start; p < &array[end]; ++p, ++count) {
		if (*p == 0x0000)
			nil_count++;
		else
			nil_count = 0;
		if (nil_count >= 3)
			continue;
		if (PRINT_AS_BINARY) {
			if (count == 0 || count % 4 == 0)
				printf("\n%05d ... %05d: ", count+1, count + 4);
			printf("0b%016b ", *p);
		} else {
			if (count == 0 || count % 4 == 0)
				printf("\n%05d ... %05d: ", count+1, count + 4);
			printf("0x%04X ", *p);
		}
	}
	printf("\n");
}

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


void destroy_byte_array(u16 *array) {
	printf("Destroying byte array...\n");
	// TODO
	// Check if array was actually allocated, or it will segfault!
	free(array);
}
#endif
