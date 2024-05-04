#include "struct.h"
#ifndef _IMPL_STRUCT_

#include <stdarg.h>
#include <stdlib.h>

static instruction_set variadic_populate_ins(int count, ...) {
	instruction_set _ISA = {0};
	va_list ap;
	va_start(ap, count);
	for (int i = 0; i < count; ++i) {
		_ISA.count++;
		_ISA.ins = (ins*)realloc(_ISA.ins, _ISA.count * sizeof(ins));
		_ISA.ins[_ISA.count - 1] = va_arg(ap, ins);
	}
	va_end(ap);
	return _ISA;
}

instruction_set init_isa() {
	return variadic_populate_ins(16,
					   CLITERAL(ins) {.opcode = 0x0000, .token = "nop"},
					   CLITERAL(ins) {.opcode = 0x0001, .token = "mov"},

					   CLITERAL(ins) {.opcode = 0x0002, .token = "add"},
					   CLITERAL(ins) {.opcode = 0x0003, .token = "sub"},

					   CLITERAL(ins) {.opcode = 0x0004, .token = "inc"},
					   CLITERAL(ins) {.opcode = 0x0005, .token = "dec"},

					   CLITERAL(ins) {.opcode = 0x0006, .token = "jmp",},
					   CLITERAL(ins) {.opcode = 0x0007, .token = "call",},
					   CLITERAL(ins) {.opcode = 0x0008, .token = "ret",},

					   CLITERAL(ins) {.opcode = 0x0009, .token = "or",},
					   CLITERAL(ins) {.opcode = 0x000a, .token = "and",},
					   CLITERAL(ins) {.opcode = 0x000b, .token = "xor",},

					   CLITERAL(ins) {.opcode = 0x000c, .token = "push",},
					   CLITERAL(ins) {.opcode = 0x000d, .token = "pop",},

					   CLITERAL(ins) {.opcode = 0x000e, .token = "halt",},
					   CLITERAL(ins) {.opcode = 0x000f, .token = "end",});
}
#endif
