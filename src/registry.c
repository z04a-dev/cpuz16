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
	_cpu->bus.cells = calloc(BUS_SIZE, sizeof(u16));
	_cpu->stack_value = &_cpu->bus.cells[ROM_START - STACK_SIZE];
	_cpu->stack_pointer = _cpu->stack_value;
	assert(_cpu->bus.cells != NULL && "buy ram");
}

struct cpu init_cpu(){
	printf("[CPUZ16] initializing register stack...\n");
	cpu _cpu = {0};
	init_memory(&_cpu);
	return _cpu;
}

void print_rom(struct cpu *_cpu) {
	u16 cols = 12;
	printf("----- ROM (size: %hu cells) contents -----\n", ROM_SIZE);
	u16 nil_count = 0;
	u16 count = 0;
	for (u16 i = ROM_START; i < BUS_SIZE; ++i, ++count) {
		if (_cpu->bus.cells[i] == 0x0000)
			nil_count++;
		else
			nil_count = 0;
		if (nil_count >= 3)
			continue;
		if (count % cols == 0 && count != 0)
			printf("\n");
		printf("[0x%04x]:0x%04x ", i, _cpu->bus.cells[i]);
		count++;
	}
	printf("\n");
}

void print_io(struct cpu *_cpu) {
	u16 cols = 12;
	printf("----- IO (size: %hu cells) contents -----\n", IO_SIZE);
	u16 nil_count = 0;
	u16 count = 0;
	for (u16 i = 0x0000; i < IO_SIZE; ++i, ++count) {
		if (_cpu->bus.cells[i] == 0x0000)
			nil_count++;
		else
			nil_count = 0;
		if (nil_count >= 3)
			continue;
		if (count % cols == 0 && count != 0)
			printf("\n");
		printf("[0x%04x]:0x%04x ", i, _cpu->bus.cells[i]);
		count++;
	}
	printf("\n");
}

void print_ram(struct cpu *_cpu) {
	u16 cols = 6;
	u16 count = 0;
	u16 nil_count = 0;
	printf("----- RAM (size: %hu cells) contents -----\n", RAM_SIZE);
	for (u16 i = RAM_START; i < (RAM_START + RAM_SIZE) - STACK_SIZE; ++i) {
		if (_cpu->bus.cells[i] == 0x0000)
			nil_count++;
		else
			nil_count = 0;
		if (nil_count >= 3)
			continue;
		if (count % cols == 0 && count != 0)
			printf("\n");
		printf("[0x%04x]:0x%04x ", i, _cpu->bus.cells[i]);
		count++;
	}
	count = 0;
	nil_count = 0;
	printf("\n----- STACK (size: %hu cells) contents -----\n", STACK_SIZE);
	for (int i = (RAM_START + RAM_SIZE) - STACK_SIZE; i < RAM_START + RAM_SIZE; ++i) {
		if (_cpu->bus.cells[i] == 0x0000)
			nil_count++;
		else
			nil_count = 0;
		if (nil_count >= 3) {
			continue;
		}
		if (count % cols == 0 && count != 0)
			printf("\n");
		printf("[0x%04x]:0x%04x ", i, _cpu->bus.cells[i]);
		count++;
	}
	printf("\n");
}

void print_cpu_state_fp(FILE *fp, struct cpu *_cpu) {
	fprintf(fp, "CPUZ16 info:\r\n");
	fprintf(fp, "rax: %5hu (0x%04x)\r\n", _cpu->rax, _cpu->rax);
	fprintf(fp, "rbx: %5hu (0x%04x)\r\n", _cpu->rbx, _cpu->rbx);
	fprintf(fp, "rdx: %5hu (0x%04x)\r\n", _cpu->rdx, _cpu->rdx);
	fprintf(fp, "a1 : %5hu (0x%04x)\r\n", _cpu->a1 , _cpu->a1);
	fprintf(fp, "a2 : %5hu (0x%04x)\r\n", _cpu->a2 , _cpu->a2);
	fprintf(fp, "a3 : %5hu (0x%04x)\r\n", _cpu->a3 , _cpu->a3);
	fprintf(fp, "ins: %5hu (0x%04x)\r\n", _cpu->ins , _cpu->ins);
	fprintf(fp, "Stack pointer: %p\r\n", (void*)_cpu->stack_pointer);
	fprintf(fp, "Instruction counter: %llu\r\n", _cpu->ic);
}

void print_cpu_state(struct cpu *_cpu) {
	printf("CPUZ16 info:\n");
	printf("rax: %5hu (0x%04x)\n", _cpu->rax, _cpu->rax);
	printf("rbx: %5hu (0x%04x)\n", _cpu->rbx, _cpu->rbx);
	printf("rdx: %5hu (0x%04x)\n", _cpu->rdx, _cpu->rdx);
	printf("a1 : %5hu (0x%04x)\n", _cpu->a1 , _cpu->a1);
	printf("a2 : %5hu (0x%04x)\n", _cpu->a2 , _cpu->a2);
	printf("a3 : %5hu (0x%04x)\n", _cpu->a3 , _cpu->a3);
	printf("ins: %5hu (0x%04x)\n", _cpu->ins , _cpu->ins);
	printf("Stack pointer: %p\n", (void*)_cpu->stack_pointer);
	// TODO return pointer
	// TODO free/max RAM/stack
	// TODO RAM pointer
	printf("Instruction counter: %llu\n\n", _cpu->ic);
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
	printf("[CPUZ16] unloading RAM\n");
	free(_cpu->bus.cells);
}

#endif
