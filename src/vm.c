/* starting point for asm-machine CPUZ16 */
/* read README.md for more info */
/* by z04a */

#include <stdio.h>
#include <stdlib.h>

#include "registry.h" 
#include "lexer.h"


void usage_panic(char *argv[]) {
	printf("err: provide path to ins.asm\n");
	printf("usage: %s <path/to/ins.asm>\n", argv[0]);
	exit(1);
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		usage_panic(argv);
	}

	cpu cpuz16 = init_cpu();	

	// print_cpu_state(&cpuz16);	
	//
	start_lexer(&cpuz16, argv[1]);

	// print_code_blocks();

	// print_ins_set();
	
	start_executing(&cpuz16);

	// print_memory(&cpuz16);
	
	free_cpu(&cpuz16);
	printf("\nVM reached end, halting...\n");
	printf("Final VM state:\n");
	print_cpu_state(&cpuz16);

	return 0;
}
