#ifndef u16
#define u16 unsigned short
#include <stdio.h>
#endif

#ifndef _REG_IMPL_
#define _REG_IMPL_
#include "struct.h"
struct cpu init_cpu(){
	printf("[CPUZ16] initializing register stack...\n");
	cpu _cpu = {0};	
	return _cpu;
}

void print_cpu_state(struct cpu *_cpu) {
	printf("CPUZ16 info:\n");
	printf("rax: %hu\n", _cpu->rax);
	printf("rbx: %hu\n", _cpu->rbx);
	printf("rdx: %hu\n", _cpu->rdx);
	printf("a1:  %hu\n", _cpu->a1);
	printf("a2:  %hu\n", _cpu->a2);
	printf("a3:  %hu\n", _cpu->a3);
	printf("Instruction counter: %hu\n\n", _cpu->ins);
}

void reset_cpu_state(struct cpu *_cpu) {
	printf("[ZCPU16] Resetting cpu...\n");
	_cpu->ins = 0;
	_cpu->rax = 0;
	_cpu->rbx = 0;
	_cpu->rdx = 0;
	_cpu->a1 = 0;
	_cpu->a2 = 0;
	_cpu->a3 = 0;
}

#endif
