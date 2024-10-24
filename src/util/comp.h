#ifndef _UTIL_COMPILER_
#define _UTIL_COMPILER_

#define NUM_CELLS 32768 /* there is 32768 cells of 16 bit values in 64 KiB */
#define MAGIC_SIZE 1 /* 1 magic cell would be enough */
/* there is no support for MAGIC sizes > 1 */

#define TOTAL_CELLS NUM_CELLS+MAGIC_SIZE

#define MAX_ROM_SIZE 65536 /* CPUZ16 can't load more than 64 KiB of memory */

#define MAGIC_VALUE 0xFF3A

#define BYTECODE_START MAGIC_SIZE /* code will start at array[MAGIC_SIZE], which will put it right after magic */

#define SWAP_ENDIANNESS false 

#include "stdio.h"

#ifndef _STRUCT_IMPL
#include "../struct.h"
#endif

#define PRINT_AS_BINARY true 

void print_byte_array(u16 *array, u16 start, u16 end) {
	u16 *p = &array[start];
	for (int count = start; p < &array[end]; ++p, ++count) {
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
#endif
