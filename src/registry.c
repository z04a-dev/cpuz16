#ifndef u16
#define u16 unsigned short
#include <stdio.h>
#endif

#ifndef _REG_IMPL_
#define _REG_IMPL_
#include "struct.h"
#include <stdlib.h>
#include <assert.h>

static void init_memory(cpu *_cpu) {
	_cpu->ram.cells = calloc(RAM_SIZE + 1, sizeof(u16));
	_cpu->stack_value = &_cpu->ram.cells[RAM_SIZE - STACK_SIZE + 1];
	_cpu->stack_pointer = _cpu->stack_value;
	assert(_cpu->ram.cells != NULL && "buy ram");
}

struct cpu init_cpu(){
	printf("[CPUZ16] initializing register stack...\n");
	cpu _cpu = {0};
	init_memory(&_cpu);
	return _cpu;
}

void print_memory(struct cpu *_cpu) {
	u16 cols = 6;
	printf("----- RAM (size: %hu) contents -----\n", RAM_SIZE);
	for (u16 i = 0; i < RAM_SIZE - STACK_SIZE + 1; ++i) {
		if (i % cols == 0 && i != 0)
			printf("\n");
		printf("[0x%04x]:0x%04x ", i, _cpu->ram.cells[i]);
	}
	printf("\n----- STACK (size: %hu) contents -----\n", STACK_SIZE);
	for (int i = RAM_SIZE - STACK_SIZE + 1; i < RAM_SIZE + 1; ++i) {
		if (i % cols == 0 && i != RAM_SIZE - STACK_SIZE - 1)
			printf("\n");
		printf("[0x%04x]:0x%04x ", i, _cpu->ram.cells[i]);
	}
	printf("\n");
}

void print_cpu_state(struct cpu *_cpu) {
	printf("CPUZ16 info:\n");
	printf("rax: %5hu (0x%04x)\n", _cpu->rax, _cpu->rax);
	printf("rbx: %5hu (0x%04x)\n", _cpu->rbx, _cpu->rbx);
	printf("rdx: %5hu (0x%04x)\n", _cpu->rdx, _cpu->rdx);
	printf("a1 : %5hu (0x%04x)\n", _cpu->a1 , _cpu->a1);
	printf("a2 : %5hu (0x%04x)\n", _cpu->a2 , _cpu->a2);
	printf("a3 : %5hu (0x%04x)\n", _cpu->a3 , _cpu->a3);
	printf("Stack pointer: %p\n", (void*)_cpu->stack_pointer);
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
	free(_cpu->ram.cells);
}

#endif
