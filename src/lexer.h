
#ifndef _LEXER_H_ 
#define _LEXER_H_
#include "struct.h"
// to execute code -> call start_lexer with path to .asm
// after that call start_executing with initialized CPUZ16
void start_lexer(cpu *_cpu, char* asm_file);
void start_executing(cpu *_cpu); 
// print_ins_set is not working at this commit.
void print_ins_set();
void print_code_blocks();
// free_blocks shouldn't be needed, because it calls itself automatically after executing
// but you may want to call it if you don't execute code for some reason ¯\_(ツ)_/¯
void free_blocks();
#endif
