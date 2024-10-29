/* Bytecode compiler for CPUZ16 */
/* by z04a */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lexer.h"
#include "isa.h"

#include "util/comp.h"
#include "arch.h"

struct compile_label {
	u16 start_ptr;
	char *label;
};

struct compile_fix {
	u16 fix_ptr;
	char *label;
};

struct compile_bytecode {
	u16 *bytecode;
	struct compile_label *labels;
	u16 labels_count;
	struct compile_fix *fixes;
	u16 fixes_count;
};

/* I've implemented check for byte array size, but please -> do not change it!*/
/* But it also may change it the future, so keeping it for now */

#define READ_BINARY false 

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

bool create_binary(u16 *array, char *filename) {
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



void append_label(struct compile_bytecode *compiler, code_block block, u16 start_pointer) {
	if (compiler->labels_count == 0) {
		compiler->labels_count++;
		compiler->labels = malloc(compiler->labels_count * sizeof(struct compile_label));
		compiler->labels[compiler->labels_count - 1].start_ptr = start_pointer;
		compiler->labels[compiler->labels_count - 1].label = block.label;
	} else {
		compiler->labels_count++;
		compiler->labels = realloc(compiler->labels, compiler->labels_count * sizeof(struct compile_label));
		compiler->labels[compiler->labels_count - 1].start_ptr = start_pointer;
		compiler->labels[compiler->labels_count - 1].label = block.label;
	}
}

void append_fix(struct compile_bytecode *compiler, u16 fix_ptr, char *label) {
	// TODO
	// This is for debug only!
	compiler->bytecode[fix_ptr] = 0xFFFF;
	//

	if (compiler->fixes_count == 0) {
		compiler->fixes_count++;
		compiler->fixes = malloc(compiler->fixes_count * sizeof(struct compile_label));
		compiler->fixes[compiler->fixes_count - 1].fix_ptr = fix_ptr;
		compiler->fixes[compiler->fixes_count - 1].label = label;
	} else {
		compiler->fixes_count++;
		compiler->fixes = realloc(compiler->fixes, compiler->fixes_count * sizeof(struct compile_label));
		compiler->fixes[compiler->fixes_count - 1].fix_ptr = fix_ptr;
		compiler->fixes[compiler->fixes_count - 1].label = label;
	}
}

void append_value(u16 *bytecell, u16 value, u16 offset, u16 length) {
	u16 value_mask = ((1<<length)-1) << offset; // for length=4 and offset=3 will be 00..01111000
	*bytecell = *bytecell & ~value_mask; // zeroing according bits inside the bytecell
	*bytecell = *bytecell | ((value << offset) & value_mask); // AND with value_mask is for extra safety
}

void append_bytearray(struct compile_bytecode *compiler, code_block block, u16 *start_pointer) {
	for (size_t i = 0; i < block.ins.count; ++i) {
		compiler->bytecode[*start_pointer] = block.ins.cmds[i].ins.opcode; //appending opcode
		
		u16 hint_count = 0;

		// appending VAL1
		if (block.ins.cmds[i].val1_type == T_VAL1_REG) {
			append_value(&compiler->bytecode[*start_pointer], block.ins.cmds[i].val1.reg, 5, 3);
		} else {
			// put HINT
			if (block.ins.cmds[i].ins.opcode != HALT_OPCODE && block.ins.cmds[i].val1_type != T_VAL1_NULL ) {
				append_value(&compiler->bytecode[*start_pointer], HINT_FLAG, 5, 3);
				hint_count++;
				if (block.ins.cmds[i].val1_type == T_VAL1_U16) {
					compiler->bytecode[*start_pointer + hint_count] = block.ins.cmds[i].val1.num;
				}
				else if (block.ins.cmds[i].val1_type == T_VAL1_LABEL)
					append_fix(compiler, *start_pointer + hint_count, block.ins.cmds[i].val1.label);
			}
		}

		// Appending VAL2
		if (block.ins.cmds[i].val2_type == T_VAL2_REG) {
			append_value(&compiler->bytecode[*start_pointer], block.ins.cmds[i].val2.reg, 8, 3);
		} else {
			// put HINT
			if (block.ins.cmds[i].ins.opcode != HALT_OPCODE && block.ins.cmds[i].val2_type != T_VAL2_NULL) {
				append_value(&compiler->bytecode[*start_pointer], HINT_FLAG, 8, 3);
				hint_count++;
				if (block.ins.cmds[i].val2_type == T_VAL2_U16) {
					// printf("Putting u16 %d\n", block.ins.cmds[i].val2.num);
					compiler->bytecode[*start_pointer + hint_count] = block.ins.cmds[i].val2.num;
				}
				else if (block.ins.cmds[i].val2_type == T_VAL2_LABEL)
					// compiler->bytecode[*start_pointer + hint_count] = 0xFFFF;
					append_fix(compiler, *start_pointer + hint_count, block.ins.cmds[i].val2.label);
			}
		}

		// TODO
		// VAL3 always is label, so no need to check for everything else
		// because it's always used in conditional jumps
		// Appending VAL3
		if (block.ins.cmds[i].val3_type == T_VAL3_REG) {
			append_value(&compiler->bytecode[*start_pointer], block.ins.cmds[i].val3.reg, 11, 3);
		} else {
			// put HINT
			if (block.ins.cmds[i].ins.opcode != HALT_OPCODE && block.ins.cmds[i].val3_type != T_VAL3_NULL) {
				append_value(&compiler->bytecode[*start_pointer], HINT_FLAG, 11, 3);
				hint_count++;
				if (block.ins.cmds[i].val3_type == T_VAL3_U16) {
					// printf("Putting u16 %d\n", block.ins.cmds[i].val3.num);
					compiler->bytecode[*start_pointer + hint_count] = block.ins.cmds[i].val3.num;
				}
				else if (block.ins.cmds[i].val3_type == T_VAL3_LABEL)
					// compiler->bytecode[*start_pointer + hint_count] = 0xFFFF;
					append_fix(compiler, *start_pointer + hint_count, block.ins.cmds[i].val3.label);
			}
		}
		// moving pointer to next cell
		*start_pointer += hint_count + 1;
	}
}


void find_start(struct compile_bytecode *compiler, code_blocks *blocks, u16 *start_pointer) {
	for (size_t i = 0 ; i < blocks->count; ++i) {
		if (strcmp(blocks->block[i].label, "start") == 0) {
			u16 start = *start_pointer;
			append_bytearray(compiler, blocks->block[i], start_pointer);
			append_label(compiler, blocks->block[i], start);
		} else continue;
	}
}

void fix_bytecode(struct compile_bytecode *compiler) {
	for (int fix = 0; fix < compiler->fixes_count; ++fix) {
		printf("Trying to fix HINT <%s>@%d...\n", compiler->fixes[fix].label, compiler->fixes[fix].fix_ptr);
		char *fix_label = compiler->fixes[fix].label;

		for (int label = 0; label < compiler->labels_count; ++label) {
			if (strcmp(fix_label, compiler->labels[label].label) == 0) {
				printf("Found fix, applying...\n");

				// Decrement MAGIC_SIZE at compile time
				// so interpreters could just drop MAGIC from their bytecode copy
				compiler->bytecode[compiler->fixes[fix].fix_ptr] = compiler->labels[label].start_ptr - MAGIC_SIZE;
				break;
			}
			if (label == compiler->labels_count - 1) {
				printf("[PANIC] Failed to found fix for this HINT\n");
				printf("[PANIC] Exiting compiler, sorry.\n");
				exit(1);
			}
		}
	}
}


void fix_define(code_blocks blocks, define_block def_block) {
	for (int i = 0; i < blocks.count; ++i) {
		for (int ins = 0; ins < blocks.block[i].ins.count; ++ins) {
			if (blocks.block[i].ins.cmds[ins].val1_type == T_VAL1_LABEL &&
					blocks.block[i].ins.cmds[ins].val1.label[0] == '@') {
				char *label = &blocks.block[i].ins.cmds[ins].val1.label[1];
				for (int def = 0; def < def_block.count; ++def) {
					if (strcmp(label, def_block.def[def].name) == 0) {
						printf("%s\n", blocks.block[i].ins.cmds[ins].ins.token);
						blocks.block[i].ins.cmds[ins].val1_type = T_VAL1_U16;
						if (def_block.def[def].def_type == T_DEF_DATA ||
								def_block.def[def].def_type == T_DEF_ASCII) {
							printf("Skipping @%s in interpreter.\n", label);
							blocks.block[i].ins.cmds[ins].val1.num = 0;
							break;
						}
						blocks.block[i].ins.cmds[ins].val1.num = def_block.def[def].value.imm;
						break;
					}
				}
			}

			if (blocks.block[i].ins.cmds[ins].val2_type == T_VAL2_LABEL &&
					blocks.block[i].ins.cmds[ins].val2.label[0] == '@') {
				char *label; 
				asprintf(&label, "%s", &blocks.block[i].ins.cmds[ins].val2.label[1]);
				printf("LABEL: %s\n", label);
				for (int def = 0; def < def_block.count; ++def) {
					printf("%s\n", def_block.def[def].name);
					if (strcmp(label, def_block.def[def].name) == 0) {
						blocks.block[i].ins.cmds[ins].val2_type = T_VAL2_U16;
						if (def_block.def[def].def_type == T_DEF_DATA ||
								def_block.def[def].def_type == T_DEF_ASCII) {
							printf("Skipping @%s in interpreter.\n", label);
							blocks.block[i].ins.cmds[ins].val2.num = 0;
							break;
						}
						blocks.block[i].ins.cmds[ins].val2.num = def_block.def[def].value.imm;
						printf("FOUND\n");
						break;
					}
				}
			}

			if (blocks.block[i].ins.cmds[ins].val3_type == T_VAL3_LABEL &&
					blocks.block[i].ins.cmds[ins].val3.label[0] == '@') {
				char *label = &blocks.block[i].ins.cmds[ins].val3.label[1];
				for (int def = 0; def < def_block.count; ++def) {
					if (strcmp(label, def_block.def[def].name) == 0) {
						blocks.block[i].ins.cmds[ins].val3_type = T_VAL3_U16;
						if (def_block.def[def].def_type == T_DEF_DATA ||
								def_block.def[def].def_type == T_DEF_ASCII) {
							printf("Skipping @%s in interpreter.\n", label);
							blocks.block[i].ins.cmds[ins].val3.num = 0;
							break;
						}
						blocks.block[i].ins.cmds[ins].val3.num = def_block.def[def].value.imm;
						break;
					}
				}
			}
		}
	}
}


void usage_panic(char *argv[]) {
	printf("err: provide path to ins.asm\n");
	printf("usage: %s <path/to/ins.asm>\n", argv[0]);
	exit(1);
}

int main(int argc, char **argv) {
	// TODO
	// add argument parser for compiler (and for interpreter too)
	if (argc == 1) {
		usage_panic(argv);
	}
	// TODO
	// Check for valid file, don't segfault!
	char *file = argv[1];

	if (READ_BINARY) {
		read_binary(file);
		return 0;
	}

	FILE *fp_check = fopen(file, "r");
	u16 fp_check_val;
	fread(&fp_check_val, sizeof(u16), 1, fp_check);
	fclose(fp_check);
	if (fp_check_val == MAGIC_VALUE) {
		printf("[ERROR] Found MAGIC, are you sure that you provided .asm file?\n");
		exit(1);
	}

	struct compile_bytecode compiler = {0};
	compiler.bytecode = create_byte_array();

	instruction_set isa = init_isa();

	code_blocks code = {.capacity = -1};
	define_block def_block = {0};
	start_lexer(&isa, file, &code);

	printf("DEF COUNT: %d\n", def_block.count);
	for (int i = 0; i < def_block.count; ++i) {
		if (def_block.def[i].def_type == T_DEF_IMM) {
			printf("[%d] %s %d\n", i, def_block.def[i].name, def_block.def[i].value.imm);
			printf("-----\n");
		}
	}

	fix_define(code, def_block);
	// there is no need to check for .start label
	// because lexer will do it for us and panic

	printf("Code blocks: %d\n", code.count);

	u16 bytecode_ptr = BYTECODE_START;
	find_start(&compiler, &code, &bytecode_ptr);

	for (size_t i = 0 ; i < code.count; ++i) {
		if (strcmp(code.block[i].label, "start") != 0) {
			u16 start = bytecode_ptr;
			append_bytearray(&compiler, code.block[i], &bytecode_ptr);
			append_label(&compiler, code.block[i], start);
			continue;
		}
	}

	// exit(0);

	printf("\n--------------------\n");
	printf("First pass completed.\n");
	if (compiler.fixes_count > 0) {
		printf("Need to fix %d instances.\n", compiler.fixes_count);
	} else {
		printf("No need to fix anything, skipping second pass.");
	}

	printf("start pointer %d\n", bytecode_ptr);

	print_byte_array(compiler.bytecode, 0, bytecode_ptr + 1);

	printf("---------------\n");
	printf("Number of labels: %d\n", compiler.labels_count);
	for (int i = 0; i < compiler.labels_count; ++i) {
		printf("[%d] -> %s (starts at: %d)\n", i, compiler.labels[i].label, compiler.labels[i].start_ptr);
	}
	printf("\n");
	printf("Number of fixes: %d\n", compiler.fixes_count);
	for (int i = 0; i < compiler.fixes_count; ++i) {
		printf("[%d] -> %s (starts at: %d)\n", i, compiler.fixes[i].label, compiler.fixes[i].fix_ptr);
	}
	printf("\n");

	fix_bytecode(&compiler);

	print_byte_array(compiler.bytecode, 0, bytecode_ptr + 1);

	printf("Successfully compiled...\n");
	u16 written_bytes = (bytecode_ptr - 2) * 2;
	printf("Written: %d bytes | Free: %d bytes\n", written_bytes, (MAX_ROM_SIZE - MAGIC_SIZE * 2) - written_bytes);
	// TODO FIX
	printf("Saving binary...\n");
	create_binary(compiler.bytecode, "output.bin");

	goto clean;

clean:
	// TODO (check if ISA was correctly initialized)
	free(isa.ins);

	if (compiler.labels_count > 0)
		free(compiler.labels);
	if (compiler.fixes_count > 0)
		free(compiler.fixes);

	destroy_byte_array(compiler.bytecode);
	return 0;
}
