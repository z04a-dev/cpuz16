/* starting point for asm-machine CPUZ16 */
/* read README.md for more info */
/* by z04a */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "registry.h" 
#include "parser.h"

#include "isa.h" /* includes struct.h */

#ifndef _SOCKET_IMPL
#include "socket.h"
#endif

#ifndef _BYTECODE_IMPL
#include "bytecode.h"
#endif

#include "util/comp-values.h"

void usage_panic(char *argv[]) {
	printf("err: provide path to ins.asm\n");
	printf("usage: %s <path/to/ins.asm>\n", argv[0]);
	exit(1);
}

// global variable Sadge
cpu cpuz16;
instruction_set isa;

void free_isa() {
	free(isa.ins);
}

void exit_func(void) {
	free_cpu(&cpuz16);
	free_isa();
}

void sig_exit(int sig) {
	exit_func();
	exit(sig);
}

#define SOCKET_PATH "./socket.sock"

int main(int argc, char *argv[]) {
	if (argc == 1) {
		usage_panic(argv);
	}

	isa = init_isa();
	cpuz16 = init_cpu();
	socket_init(&cpuz16, SOCKET_PATH);

	// that's cool!
	atexit(exit_func);
    signal(SIGINT, sig_exit);
    signal(SIGKILL, sig_exit);
    signal(SIGABRT, sig_exit); 

	FILE *fp_check = fopen(argv[1], "r");
	u16 fp_check_val;
	fread(&fp_check_val, sizeof(u16), 1, fp_check);
	fclose(fp_check);
	if (fp_check_val == MAGIC_VALUE) {
		cpuz16.state = VM_BINARY;
		cpuz16.ins = ROM_START;
		printf("[CPUZ16] Found MAGIC, starting BINARY mode...\n");
		u16 *bytearray = read_binary(argv[1]);
		memcpy(&cpuz16.bus.cells[ROM_START], bytearray, ROM_SIZE * 2);
		parse_bytecode(&isa, &cpuz16);
		goto end;
	} 

	printf("[CPUZ16] MAGIC not found, starting INTERPRETER mode...\n");
	cpuz16.state = VM_INTERPRETER;

	// print_cpu_state(&cpuz16);

	// TODO
	// need to free code blocks and def blocks
	code_blocks code = {.capacity = -1};
	define_block def = {0};
	start_parser(&isa, argv[1], &code, &def);

	if (def.count > 0) {
		printf("@DEFINES are not supported in interpreter mode.\n");
		return -1;
	}

	// print_code_blocks(&code);

	// print_ins_set();
	
	start_interpreter(&cpuz16, &code);

end:
	// print_io(&cpuz16);
	// print_ram(&cpuz16);
	print_rom(&cpuz16);
	// printf("STACK_SIZE: %d\n", STACK_SIZE);
	// printf("IO_SIZE: %d\n", IO_SIZE);
	// printf("\nRAM_SIZE: %d\n", RAM_SIZE);
	// printf("RAM_START: %d\n", RAM_START);
	// printf("ROM_START: %d\n", ROM_START);
	// printf("ROM_SIZE: %d\n", ROM_SIZE);
	// printf("\nBUS_SIZE: %d\n", BUS_SIZE);
	// printf("\nIO[0x%04X->0x%04X]\n", 0, IO_SIZE);
	// printf("RAM[0x%04X->0x%04X]\n", RAM_START, RAM_START+RAM_SIZE);
	// printf("ROM[0x%04X->0x%04X]\n", ROM_START, BUS_SIZE);

	printf("\nVM reached end, halting...\n");
	printf("Final VM state:\n");
	print_cpu_state(&cpuz16);

	return 0;
}
