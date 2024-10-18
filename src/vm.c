/* starting point for asm-machine CPUZ16 */
/* read README.md for more info */
/* by z04a */

#include <stdio.h>
#include <stdlib.h>

#include "registry.h" 
#include "lexer.h"

#include "isa.h" /* includes struct.h */

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

int main(int argc, char *argv[]) {
	if (argc == 1) {
		usage_panic(argv);
	}

	
	isa = init_isa();
	cpuz16 = init_cpu();

	// print_cpu_state(&cpuz16);	

	code_blocks code = {.capacity = -1};
	start_lexer(&isa, argv[1], &code);

	// print_code_blocks();

	// print_ins_set();
	
	// that's cool!
	atexit(exit_func);
	
	start_interpreter(&cpuz16, &code);

	// print_memory(&cpuz16);
	
	printf("\nVM reached end, halting...\n");
	printf("Final VM state:\n");
	print_cpu_state(&cpuz16);

	return 0;
}
