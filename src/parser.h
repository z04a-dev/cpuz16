
#ifndef _PARSER_H_ 
#define _PARSER_H_
#include "struct.h"
// to execute code -> call start_parser with path to .asm
// after that call start_interpreter with initialized CPUZ16
void start_parser(instruction_set *_isa, char* asm_file, code_blocks *out_blocks, define_block *out_def);
void start_interpreter(cpu *_cpu, code_blocks *blocks);
// TODO: maybe delete it?
// print_ins_set is not working at this commit.
void print_ins_set();
void print_code_blocks(code_blocks *blocks);
// free_blocks shouldn't be needed, because it calls itself automatically after executing
// but you may want to call it if you don't execute code for some reason ¯\_(ツ)_/¯
void free_blocks(code_blocks *blocks);
#endif
