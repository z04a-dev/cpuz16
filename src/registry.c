#ifndef u16
#define u16 unsigned short
#include <stdio.h>
#endif


#ifndef _REG_IMPL_
#define _REG_IMPL_
#include "struct.h"
#include <stdlib.h>
#include <assert.h>

struct cpu init_cpu(){
	printf("[CPUZ16] initializing register stack...\n");
	cpu _cpu = {0};	
	_cpu.stack_value = calloc(STACK_SIZE, sizeof(u16));
	assert(_cpu.stack_value != NULL && "buy ram");
	_cpu.stack_pointer = _cpu.stack_value;
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
	// printf("Stack pointer: %p\n", _cpu->stack.pointer);
	printf("Instruction counter: %hu\n\n", _cpu->ins);
}

void reset_cpu_state(struct cpu *_cpu) {
	printf("[CPUZ16] Resetting cpu...\n");
	_cpu->ins = 0;
	_cpu->rax = 0;
	_cpu->rbx = 0;
	_cpu->rdx = 0;
	_cpu->a1 = 0;
	_cpu->a2 = 0;
	_cpu->a3 = 0;
}

void free_cpu(struct cpu *_cpu) {
	printf("[CPUZ16] unloading stack memory\n");
	free(_cpu->stack_value);
}

#endif
