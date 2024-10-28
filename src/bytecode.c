#ifndef _STRUCT_IMPL
#include "struct.h"
#endif

#include <time.h>

#define SEC_TO_MS(sec) ((sec)*1000000)

#include "util/comp.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "util/to_str.h"
#include "instruction.h"

#define SECS_TO_NS(SECS) (SECS * 1000000000.)

#define DEBUG 0 // Print instructions and execute
#define DEBUG_PRINT 0 // Don't execute instructions, just print them. (wouldn't HALT)

#define CLOCK SECS_TO_NS((1. / 1000000000.))

static u16 *vm_create_bytearray() {
	unsigned int byte_array_size = (NUM_CELLS) * sizeof(u16);
	if (!check_array_size(byte_array_size)) {
		printf("PANIC: Failed to allocate byte array!\n");
		exit(1);
	}
	printf("Allocating byte array<%d>...\n", byte_array_size); 

	/* Using calloc, so array gets initialied with 0 */
	// u16 *array = malloc(byte_array_size);
	u16 *array = calloc(NUM_CELLS, sizeof(u16));
	if (array == NULL) {
		printf("PANIC: Failed to allocate bytecode\n");
		printf("PANIC: Are you out of memory? calloc() failed!\n");
		exit(1);
	}
	return array;
}

u16 *read_binary(char *file) {

	FILE *fp;
	fp = fopen(file, "rb");
	if (fp == NULL) {
		printf("PANIC: Failed to open <%s>...\n", file);
		exit(1);
	}
	u16 *array = vm_create_bytearray();
	u16 *p = array;
	size_t offset_in_bytes = MAGIC_SIZE * 2; // start after MAGIC
	fseek(fp, offset_in_bytes, 0);
	for (; p < &array[NUM_CELLS]; ++p) {
		if (fread(p, sizeof(u16), 1, fp) != 1)
			break;
	}
	fclose(fp);
	// print_byte_array(array, 0, NUM_CELLS);
	// destroy_byte_array(array);
	return array;
}

// Yoinked from https://www.geeksforgeeks.org/extract-k-bits-given-position-number/
// Function to extract k bits from p position
// and returns the extracted value as integer
static int bitExtracted(int number, int k, int p)
{
    return (((1 << k) - 1) & (number >> (p - 1)));
}

static struct instruction _recognize_ins(instruction_set *_isa, u16 _token) {
	assert(_isa->count > 0 && "initialize ISA first!");
	for (unsigned int i = 0; i < _isa->count; ++i) {
		if (_token == _isa->ins[i].opcode) {
			return _isa->ins[i];
		}
	}	
	return _isa->ins[0]; /* return nop */
}

static bool if_jmp(struct instruction instr) {
	switch (instr.opcode) {
		case 0x0006:
			return true;
			break;
		case 0x0007:
			return true;
			break;
		case 0x0010 ... 0x0015:
			return true;
			break;
	}
	return false;
}

// Compiler decrements label pos by MAGIC_SIZE
void parse_bytecode(u16 *bytecode, instruction_set *_isa, cpu *_cpu) {
	// unsigned long CLOCK = SEC_TO_MS(1. / 1000000.);
	// printf("CLOCK: %f\n", CLOCK);
	for (;;) {
		u16 i = _cpu->ins;
		u16 hint_count = 0;
		u16 cell = bytecode[i];

		cmd _cmd = {0};
		struct instruction instr = _recognize_ins(_isa, cell & 0x1F);
		_cmd.ins = instr;
		if (DEBUG)
			printf("FULL [0x%04x] 0x%016b | INSTR: %s |", (unsigned int)i, cell, instr.token);
		u16 val1 = bitExtracted(cell, 3, 6);
		if (val1 == 0x07) {
			hint_count++;
			val1 = bytecode[i+hint_count];
			_cmd.val1_type = T_VAL1_U16;
			_cmd.val1.num = val1;
			if (DEBUG)
				printf(" VAL1: %d |", val1);
		} else {
			_cmd.val1_type = T_VAL1_REG;
			_cmd.val1.reg = val1;
			if (DEBUG)
				printf(" VAL1: %s |", reg_to_str(val1));
		}

		u16 val2 = bitExtracted(cell, 3, 9);

		if (val2 == 0x07) {
			hint_count++;
			val2 = bytecode[i+hint_count];
			_cmd.val2_type = T_VAL2_U16;
			_cmd.val2.num = val2;
			if (DEBUG)
				printf(" VAL2: %d |", val2);
		} else {
			_cmd.val2_type = T_VAL2_REG;
			_cmd.val2.reg = val2;
			if (DEBUG)
				printf(" VAL2: %s |", reg_to_str(val2));
		}

		u16 val3 = bitExtracted(cell, 3, 12);
		if (val3 == 0x07) {
			hint_count++;
			val3 = bytecode[i+hint_count];
			_cmd.val3_type = T_VAL3_U16;
			_cmd.val3.num = val3;
			if (DEBUG)
				printf(" VAL3: %d | TOTAL: %llu\n", val3, _cpu->ic);
		} else {
			_cmd.val3_type = T_VAL3_REG;
			_cmd.val3.reg = val3;
			if (DEBUG)
				printf(" VAL3: %s | TOTAL: %llu\n", reg_to_str(val3), _cpu->ic);
		}

		// printf("FULL [0x%04x]: 0x%016b | INSTR: %s | VAL1: 0x%03b | VAL2: 0x%03b | VAL3: 0x%03b\n", (unsigned int)i, cell, instr.token, val1, val2, val3);
		if (DEBUG && !DEBUG_PRINT)
			for (size_t hint = 0; hint < hint_count; ++hint) {
				printf("HINT [0x%04x]: FULL: 0x%016b | SKIPPING...\n", (unsigned int)(i + (hint + 1)), bytecode[i + (hint + 1)]);
			}

		// Wait for clock
		// usleep(CLOCK);
		// nanosleep((const struct timespec[]){{0, CLOCK}}, NULL);
		
		if (DEBUG_PRINT)
			_cpu->ins += hint_count + 1;
		if (!DEBUG_PRINT) {
			_cpu->ic++;
			int status = execute_instruction(_cpu, &_cmd, NULL);
				if (status == 2)
					_cpu->ins += hint_count + 1;
				if (status == -1) {
					printf("[PANIC] Something panicked, exiting...\n");
					break;
				}

			if (!if_jmp(instr))
				_cpu->ins += hint_count + 1; // move to next instruction if not jumping
		}
	} 
}
