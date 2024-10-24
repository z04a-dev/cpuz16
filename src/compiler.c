/* This is test-field for creating a compiler for CPUZ16 */
/* by z04a */

/* This is not working now! Currently in the process of refactoring lexer logic */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lexer.h"
#include "isa.h"

#include "util/comp.h"
#include "arch.h"

/* I've implemented check for byte array size, but please -> do not change it!*/
/* But it also may change it the future, so keeping it for now */

#define READ_BINARY false

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

void read_binary(char *file) {
	FILE *fp;
	fp = fopen(file, "r");
	if (fp == NULL) {
		printf("PANIC: Failed to open <%s>...\n", file);
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


void usage_panic(char *argv[]) {
	printf("err: provide path to ins.asm\n");
	printf("usage: %s <path/to/ins.asm>\n", argv[0]);
	exit(1);
}

void append_value(u16 *bytecell, u16 value, u16 offset, u16 length) {
	// printf("APPENDING VALUE : %d\n", value);
	u16 value_mask = ((1<<length)-1) << offset; // for length=4 and offset=3 will be 00..01111000
	*bytecell = *bytecell & ~value_mask; // zeroing according bits inside the bytecell
	*bytecell = *bytecell | ((value << offset) & value_mask); // AND with value_mask is for extra safety
}

void append_bytearray(u16 *bytecode, code_block block, u16 *start_pointer) {
	// printf("APPENDING %s at %d\n", block.label, *start_pointer);
	// TODO
	// add start_pointer of each block to some array
	// so it can be used in fixes for jumps
	for (size_t i = 0; i < block.ins.count; ++i) {
		// printf("Putting: %s\n", block.ins.cmds[i].ins.token);
		bytecode[*start_pointer] = block.ins.cmds[i].ins.opcode; //appending opcode
		
		u16 hint_count = 0;

		if (block.ins.cmds[i].val2_type == T_VAL2_NULL) {
			printf("%s -> NULL TYPE\n", block.ins.cmds[i].ins.token);
		}
		// appending VAL1
		if (block.ins.cmds[i].val1_type == T_VAL1_REG) {
			append_value(&bytecode[*start_pointer], block.ins.cmds[i].val1.reg, 5, 3);
		} else {
			// put HINT
			if (block.ins.cmds[i].ins.opcode != HALT_OPCODE && block.ins.cmds[i].val1_type != T_VAL1_NULL ) {
				append_value(&bytecode[*start_pointer], HINT_FLAG, 5, 3);
				hint_count++;
				if (block.ins.cmds[i].val1_type == T_VAL1_U16) {
					printf("Putting u16 %d\n", block.ins.cmds[i].val1.num);
					bytecode[*start_pointer + hint_count] = block.ins.cmds[i].val1.num;
				}
				else if (block.ins.cmds[i].val1_type == T_VAL1_LABEL)
					bytecode[*start_pointer + hint_count] = 0xFFFF;
				else if (block.ins.cmds[i].val1_type == T_VAL1_ADDRESS) {
					printf("it shouldn't be ADDRESS, Please FIX!!!!!!\n");
					bytecode[*start_pointer + hint_count] = 0xFFFF;
				}
			}
		}

		// Appending VAL2
		if (block.ins.cmds[i].val2_type == T_VAL2_REG) {
			append_value(&bytecode[*start_pointer], block.ins.cmds[i].val2.reg, 8, 3);
		} else {
			// put HINT
			if (block.ins.cmds[i].ins.opcode != HALT_OPCODE && block.ins.cmds[i].val2_type != T_VAL2_NULL) {
				append_value(&bytecode[*start_pointer], HINT_FLAG, 8, 3);
				hint_count++;
				if (block.ins.cmds[i].val2_type == T_VAL2_U16) {
					printf("Putting u16 %d\n", block.ins.cmds[i].val2.num);
					bytecode[*start_pointer + hint_count] = block.ins.cmds[i].val2.num;
				}
				else if (block.ins.cmds[i].val2_type == T_VAL2_LABEL)
					bytecode[*start_pointer + hint_count] = 0xFFFF;
				else if (block.ins.cmds[i].val2_type == T_VAL2_ADDRESS) {
					printf("it shouldn't be ADDRESS, Please FIX!!!!!!\n");
					bytecode[*start_pointer + hint_count] = 0xFFFF;
				}
			}
		}

		// TODO
		// VAL3 always is label, so no need to check for everything else
		// because it's always used in conditional jumps
		// Appending VAL3
		if (block.ins.cmds[i].val3_type == T_VAL3_REG) {
			append_value(&bytecode[*start_pointer], block.ins.cmds[i].val3.reg, 11, 3);
		} else {
			// put HINT
			if (block.ins.cmds[i].ins.opcode != HALT_OPCODE && block.ins.cmds[i].val3_type != T_VAL3_NULL) {
				append_value(&bytecode[*start_pointer], HINT_FLAG, 11, 3);
				hint_count++;
				if (block.ins.cmds[i].val3_type == T_VAL3_U16) {
					printf("Putting u16 %d\n", block.ins.cmds[i].val3.num);
					bytecode[*start_pointer + hint_count] = block.ins.cmds[i].val3.num;
				}
				else if (block.ins.cmds[i].val3_type == T_VAL3_LABEL)
					bytecode[*start_pointer + hint_count] = 0xFFFF;
				else if (block.ins.cmds[i].val3_type == T_VAL3_ADDRESS) {
					printf("it shouldn't be ADDRESS, Please FIX!!!!!!\n");
					bytecode[*start_pointer + hint_count] = 0xFFFF;
				}
			}
		}
		// moving pointer to next cell
		*start_pointer += hint_count + 1;
	}
}

void find_start(u16 *bytecode, code_blocks *blocks, u16 *start_pointer) {
	for (size_t i = 0 ; i < blocks->count; ++i) {
		if (strcmp(blocks->block[i].label, "start") == 0) {
			append_bytearray(bytecode, blocks->block[i], start_pointer);
		} else continue;
	}
}

int main(int argc, char **argv) {
	if (argc == 1) {
		usage_panic(argv);
	}
	// TODO
	// Check for valid file, don't segfault!
	char *file = argv[1];
	printf("ERROR: COMPILER IS NOT FINISHED. EXITING...\n");
	// return(1);

	if (READ_BINARY) {
		read_binary(file);
		return 0;
	}

	u16 *bytecode = create_byte_array();


	instruction_set isa = init_isa();

	code_blocks code = {.capacity = -1};
	start_lexer(&isa, argv[1], &code);
	// there is no need to check for .start label
	// because lexer will do it for us and panic

	printf("Code blocks: %d\n", code.count);

	u16 bytecode_ptr = BYTECODE_START;
	find_start(bytecode, &code, &bytecode_ptr);

	for (size_t i = 0 ; i < code.count; ++i) {
		if (strcmp(code.block[i].label, "start") != 0) {
			// printf("Code block %zu:\n", i);
			// printf("Name: %s\n", code.block[i].label);
			append_bytearray(bytecode, code.block[i], &bytecode_ptr);
			printf("\n");
			continue;
		}
	}

	printf("start pointer %d\n", bytecode_ptr);

	/* REMOVE: This is just for testing binary */

	// bytecode[BYTECODE_START] = 0x1717;

	/* REMOVE to here */

	print_byte_array(bytecode, 0, 62);

	// printf("Creating binary...\n");
	// create_binary(bytecode);
	goto clean;

clean:
	// TODO (check if ISA was correctly initialized)
	free(isa.ins);
	destroy_byte_array(bytecode);
	return 0;
}
