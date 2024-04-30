
#ifndef _LEXER_H_ 
#define _LEXER_H_
#include "struct.h"
void start_lexer(char* asm_file);
void start_executing(cpu *_cpu); 
void print_ins_set();
void reset_ins_pool(); 
#endif
