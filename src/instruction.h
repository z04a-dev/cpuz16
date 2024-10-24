#ifndef _INS_H_
#define _INS_H_

#ifndef _STRUCT_IMPL
#include "struct.h"
#endif

void ins_dbg_print(); /* turn on/off INS printing */

// int execute_instruction(cpu *_cpu, cmd *_cmd);
int execute_instruction(cpu *_cpu, cmd *_cmd, code_blocks *_code_blocks); 

int execute_interpreter(cpu *_cpu, code_blocks *_code_blocks); 

void print_stack(cpu *_cpu); 

int pop_stack(cpu *_cpu, u16 *_value);
// char *ins_to_str(enum INSTRUCTION ins);

#endif
