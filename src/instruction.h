#ifndef u16
#define u16 unsigned short
#endif

#ifndef _INS_H_
#define _INS_H_
#include "struct.h"

void ins_dbg_print(); /* turn on/off INS printing */

int execute_instruction(cpu *_cpu, cmd _cmd, code_blocks *_code_blocks, instruction_pointer *_ip); 
int execute_code(cpu *_cpu, code_blocks *_code_blocks, instruction_pointer *_ip); 

#endif
