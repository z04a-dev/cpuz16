#include "struct.h"
#ifndef _IMPL_STRUCT_

#include <stdarg.h>
#include <stdlib.h>

#include "arch.h"

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
	return variadic_populate_ins(28 /* <- DONT FORGET TO UPDATE */,
					   CLITERAL(ins) {.opcode = NOP_OPCODE, .token = "nop"},
					   CLITERAL(ins) {.opcode = MOV_OPCODE, .token = "mov"},

					   CLITERAL(ins) {.opcode = ADD_OPCODE, .token = "add"},
					   CLITERAL(ins) {.opcode = SUB_OPCODE, .token = "sub"},

					   CLITERAL(ins) {.opcode = INC_OPCODE, .token = "inc"},
					   CLITERAL(ins) {.opcode = DEC_OPCODE, .token = "dec"},

					   CLITERAL(ins) {.opcode = JMP_OPCODE, .token = "jmp",},
					   CLITERAL(ins) {.opcode = CALL_OPCODE, .token = "call",},
					   CLITERAL(ins) {.opcode = RET_OPCODE, .token = "ret",},

					   CLITERAL(ins) {.opcode = OR_OPCODE, .token = "or",},
					   CLITERAL(ins) {.opcode = AND_OPCODE, .token = "and",},
					   CLITERAL(ins) {.opcode = XOR_OPCODE, .token = "xor",},

					   CLITERAL(ins) {.opcode = PUSH_OPCODE, .token = "push",},
					   CLITERAL(ins) {.opcode = POP_OPCODE, .token = "pop",},

					   CLITERAL(ins) {.opcode = HALT_OPCODE, .token = "halt",},
					   CLITERAL(ins) {.opcode = END_OPCODE, .token = "end",},

					   /* conditional jmp ISA extension */
					   CLITERAL(ins) {.opcode = JEQ_OPCODE, .token = "jeq",},
					   CLITERAL(ins) {.opcode = JNE_OPCODE, .token = "jne",},
					   CLITERAL(ins) {.opcode = JGT_OPCODE, .token = "jgt",},
					   CLITERAL(ins) {.opcode = JLT_OPCODE, .token = "jlt",},
					   CLITERAL(ins) {.opcode = JGE_OPCODE, .token = "jge",},
					   CLITERAL(ins) {.opcode = JLE_OPCODE, .token = "jle",},
					   /* memory access ISA extension */
					   CLITERAL(ins) {.opcode = LV_OPCODE, .token = "lv",},
					   CLITERAL(ins) {.opcode = SV_OPCODE, .token = "sv",},
					   /* math ISA extension */
					   CLITERAL(ins) {.opcode = MUL_OPCODE, .token = "mul",},
					   CLITERAL(ins) {.opcode = DIV_OPCODE, .token = "div",},
					   /* bitshift ISA extension */
					   CLITERAL(ins) {.opcode = ROR_OPCODE, .token = "ror",},
					   CLITERAL(ins) {.opcode = ROL_OPCODE, .token = "rol",});

}
#endif
