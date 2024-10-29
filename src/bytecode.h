#ifndef _STRUCT_IMPL
#include "struct.h"
#endif

#define _BYTECODE_IMPL

u16 *read_binary(char *file);

void parse_bytecode(instruction_set *isa, cpu *_cpu);
