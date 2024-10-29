#ifdef VM_BUILD
#include "instruction.h"
#endif

#ifndef _STRUCT_IMPL
#include "struct.h"
#endif

#ifndef _LEXER_IMPL_
#define _LEXER_IMPL_
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arch.h"

#ifdef VM_BUILD
#include "util/to_str.h"
#endif

#define LEXER_DEBUG_FILE false // debug at parsing
#define LEXER_DEBUG_INSTRUCTIONS false // enable execute_instructions debug function
#define LEXER_DEBUG_STACK false // print out stack 

// TODO i guess i dont need REG_INS here
// maybe i do...
// MAYBE use it as PC when executing bytecode?
char *REG_NAMES[REGISTRY_COUNT] = {"rax",
	"rbx",
	"rdx",
	"a1",
	"a2",
	"a3",
	"ins"};


void free_blocks(code_blocks *_blocks) {
	printf("[VM] freeing blocks\n");
	if (_blocks->count != 0) {
		for (u16 i = 0; i < _blocks->count; ++i) {
			if (_blocks->block[i].ins.count != 0) {
				free(_blocks->block[i].ins.cmds);
			}
		}
		free(_blocks->block);
	}
}

static void add_ins_to_pool(instruction_pool *pool, cmd _cmd) {
	if (pool->count == pool->capacity) {
		printf("[ERROR] ROM overflow");
		exit(1);
	}
	pool->count++;
	pool->cmds = realloc(pool->cmds, pool->count * sizeof(cmd));
	pool->cmds[pool->count - 1] = _cmd;
}

static char *remove_start_whitespaces(char *str) {
	unsigned int index = 0;
	while (str[index] == ' ' || str[index] == '\t') {
		index++;
	}
	str = &str[index];
	return str; 
}

static bool is_comment(char *str) {
	char *ptr = strstr(str, ";;");
	if (ptr == NULL) {
		return false;
	}
	if (*ptr == str[0]) {
		return true;
	} else if (*(ptr-1) == ' '){
		*ptr = '\0';
	} else {
		*(ptr+1) = '\0';
	}
	return false;
} 

static bool _recognize_ins(instruction_set *_isa, cmd *_cmd, char *_token) {
	assert(_isa->count > 0 && "initialize ISA first!");
	for (unsigned int i = 0; i < _isa->count; ++i) {
		if (strcmp(_token, _isa->ins[i].token) == 0) {
			_cmd->ins = _isa->ins[i];
			return true;
		}
	}	
	_cmd->ins = _isa->ins[0]; /* return nop */
	return false;
}

#ifdef VM_BUILD
static void debug_print_cmd(cmd *_cmd) {
	char *_v1;
	if (_cmd->val1_type == T_VAL1_U16)
		asprintf(&_v1, "%hu", _cmd->val1.num);
	else if (_cmd->val1_type == T_VAL1_REG)
		asprintf(&_v1, "%s", reg_to_str(_cmd->val1.reg));
	else if (_cmd->val1_type == T_VAL1_LABEL)
		asprintf(&_v1, "%s", _cmd->val1.label);

	char *_v2;
	if (_cmd->val2_type == T_VAL2_U16)
		asprintf(&_v2, "%hu", _cmd->val2.num);
	else if (_cmd->val2_type == T_VAL2_REG)
		asprintf(&_v2, "%s", reg_to_str(_cmd->val2.reg));
	else if (_cmd->val2_type == T_VAL2_LABEL)
		asprintf(&_v2, "%s", _cmd->val2.label);

	char *_v3;
	if (_cmd->val3_type == T_VAL3_U16)
		asprintf(&_v3, "%hu", _cmd->val3.num);
	else if (_cmd->val3_type == T_VAL3_REG)
		asprintf(&_v3, "%s", reg_to_str(_cmd->val3.reg));
	else if (_cmd->val3_type == T_VAL3_LABEL)
		asprintf(&_v3, "%s", _cmd->val3.label);

	printf("[DEBUG] cmd: %s val1: %s val2: %s val3: %s", _cmd->ins.token, _v1, _v2, _v3);
}
#endif

static bool is_token_registry(char *token,int *reg) {
	for (int i = 0; i < REGISTRY_COUNT; ++i)
		if (strcmp(token, REG_NAMES[i]) == 0) {
			*reg = i;
			return true;
		}
	return false;
}

static bool is_token_hex(char *token) {
	if (token[0] == '#' && strlen(token) <= 5 && strlen(token) > 1) {
		// TODO implement checking for bad values
		// such as #00GJ
		// 0 - 9 A - F
		return true;
	}
	return false;
}

static void populate_code_blocks(code_block _block, code_blocks *out_blocks) {
	if (out_blocks->count == 65535) {
		printf("[PANIC] too much code blocks (must be less than 65535)");
		exit(1);
	}
	out_blocks->count++;
	out_blocks->block = realloc(out_blocks->block, out_blocks->count * sizeof(code_block));
	out_blocks->block[out_blocks->count - 1] = _block;
}

static void clear_token(char *_token) {
	char *p_trash;
	p_trash = strrchr(_token, ',');
	if (p_trash == NULL) {
		p_trash = strrchr(_token, ';');
	}
	if (p_trash != NULL) {
		*p_trash = '\0';
	}
}


static void _recognize_value(char *str, cmd *_cmd, int arg) {
	if (_cmd->ins.opcode == JMP_OPCODE || _cmd->ins.opcode == CALL_OPCODE) {
				_cmd->val1_type = T_VAL1_LABEL;
				_cmd->val2_type = T_VAL2_NULL;
				_cmd->val3_type = T_VAL3_NULL;
				asprintf(&_cmd->val1.label, "%s", str);
	} else {
		int reg = 0;
		switch (arg) {
			case 1:
				// if (strlen(str) > 1 && str[0] == '*') {
				// 	_cmd->val1_conv_addr = true;
				// 	str = &str[1];
				// }
				if (is_token_registry(str, &reg)) {
					// TODO 
					// maybe it's a good idea to allow incrementing IC?
					// that will allow skipping instructions
					assert(!(_cmd->ins.opcode == INC_OPCODE && reg == INS_REGISTRY) && "You can't increment IC manually.");
					_cmd->val1_type = T_VAL1_REG;
					_cmd->val1.reg = reg; 
				} else if (is_token_hex(str)) {
					str = &str[1];
					_cmd->val1_type = T_VAL1_U16;
					_cmd->val1.num = (u16)strtol(str, NULL, 16);
				} else {
					_cmd->val1_type = T_VAL1_U16;
					_cmd->val1.num = (u16)atoi(str);
				}
				break;
			case 2:
				// if (strlen(str) > 1 && str[0] == '*') {
				// 	_cmd->val2_conv_addr = true;
				// 	str = &str[1];
				// }
				if (is_token_registry(str, &reg)) {
					_cmd->val2_type = T_VAL2_REG;
					_cmd->val2.reg = reg; 
				} else if (is_token_hex(str)) {
					str = &str[1];
					_cmd->val2_type = T_VAL2_U16;
					_cmd->val2.num = (u16)strtol(str, NULL, 16);
				} else {
					_cmd->val2_type = T_VAL2_U16;
					_cmd->val2.num = (u16)atoi(str);
				}
				break;
			case 3:
				// if (strlen(str) > 1 && str[0] == '*') {
				// 	_cmd->val3_conv_addr = true;
				// 	str = &str[1];
				// }
				// if (_cmd->ins.opcode == CONDITIONAL JUMP) TODO
				if (_cmd->ins.opcode == JEQ_OPCODE ||
						_cmd->ins.opcode == JNE_OPCODE ||
						_cmd->ins.opcode == JGT_OPCODE ||
						_cmd->ins.opcode == JLT_OPCODE ||
						_cmd->ins.opcode == JGE_OPCODE ||
						_cmd->ins.opcode == JLE_OPCODE) {
					_cmd->val3_type = T_VAL3_LABEL;
					asprintf(&_cmd->val3.label, "%s", str);
				} else {
					if (is_token_registry(str, &reg)) {
						_cmd->val3_type = T_VAL3_REG;
						_cmd->val3.reg = reg; 
					} else if (is_token_hex(str)) {
						str = &str[1];
						_cmd->val3_type = T_VAL3_U16;
						_cmd->val3.num = (u16)strtol(str, NULL, 16);
					} else {
						_cmd->val3_type = T_VAL3_U16;
						_cmd->val3.num = (u16)atoi(str);
					}
				}
				break;
		}
	}
}

static cmd _tokenize_str(instruction_set *_isa, char *str) {
	char *DEBUG_STR;
	asprintf(&DEBUG_STR, "%s", str);

	cmd _cmd = {0};
	char *token;
	token = strtok(str, " ");
	unsigned int pos = 0;
	do {
		clear_token(token);
		// printf("TOKEN AFTER CLEARING: %s\n", token);
		switch (pos) {
			case 0: /* ins */ 
				assert(_recognize_ins(_isa, &_cmd, token) && "Illegal instruction");
				break;
			case 1: /* val1 */
				_recognize_value(token, &_cmd, 1);
				break;
			case 2: /* val2 */
				_recognize_value(token, &_cmd, 2);
				break;
			case 3: /* val3 */
				_recognize_value(token, &_cmd, 3);
				break;
			default:
				assert(0 && "Instruction can't handle more than 3 arguments.");
				break;
		}
		token = strtok(NULL, " ");	
		pos++;
	} while (token != NULL);

	// hmmmmmm
	// if (pos == 1 && (_cmd.ins.opcode != END_OPCODE || _cmd.ins.opcode != NOP_OPCODE || _cmd.ins.opcode != HALT_OPCODE)) {
	// 	assert(0 && "you can't provide no arguments to instructions other than nop and halt");
	// }

#ifdef VM_BUILD
	if (LEXER_DEBUG_FILE) {
		printf("line: %s ", DEBUG_STR);
		debug_print_cmd(&_cmd);
		// printf("\n");
		printf(" || OPCODE: %s 0x%04x\n", _cmd.ins.token, _cmd.ins.opcode);
	}
#endif
	free(DEBUG_STR);
	return _cmd;
}

static bool process_str(instruction_set *_isa, char *str, code_block *block) {
	char *end_ptr = strrchr(str, '\n');
	if (end_ptr != NULL) {
		*end_ptr = '\0';
	}
	if (strlen(str) == 0)
		return true;
	str = remove_start_whitespaces(str);
	if (is_comment(str))
		return true;
	if (LEXER_DEBUG_FILE)
		printf("[%d] ", block->ins.count);
	cmd _cmd = _tokenize_str(_isa, str);
	if (_cmd.ins.opcode == END_OPCODE)
		return false;
	add_ins_to_pool(&block->ins, _cmd);
	return true;
}

static bool check_for_start(code_blocks *_blocks) {
	for (u16 i = 0; i < _blocks->count; ++i) {
		if (strcmp(_blocks->block[i].label, "start") == 0) {
			// append HALT if it's not already present
			// maybe we don't need it?....
			// TODO
			bool halt_presence = false;
			for (u16 j = 0; j < _blocks->block[i].ins.count; ++j) {
				if (_blocks->block[i].ins.cmds[j].ins.opcode == HALT_OPCODE) {
					halt_presence = !halt_presence;
					break;
				}
			}
			if (!halt_presence) {
				cmd _haltcmd = {.ins.opcode = HALT_OPCODE, .ins.token = "halt"};
				add_ins_to_pool(&_blocks->block[i].ins, _haltcmd);
			}
				
			return true;
		}
	}
	return false;
}

void start_lexer(instruction_set *_isa, char *asm_file, code_blocks *out_blocks) {
	if (access(asm_file, F_OK) != 0)
		assert(0 && ".asm file does not exist");
	printf("[Lexer] starting lexer at %s\n", asm_file);

	FILE* asm_f;
	asm_f = fopen(asm_file, "r");
	/* wow that's a lot of char* */
	/* thats dumb :) */
	char line[512];
	char *t_line;
	char *token;
	char *pColumn;
	while (fgets(line, 512, asm_f)) {
		asprintf(&t_line, "%s", line);
		t_line = remove_start_whitespaces(t_line);
		token = strtok(t_line, " ");
		pColumn = strrchr(t_line, ':');
		if (pColumn == NULL || strlen(token) <= 2)
			continue;
		*pColumn = '\0';
		code_block block = {.ins = {.capacity = -1}};
		asprintf(&block.label, "%s", token);
		bool eof = false;
		do {
			if (!fgets(line, 512, asm_f)) {
				eof = true;
				break;
			}
		} while (process_str(_isa, line, &block));
		populate_code_blocks(block, out_blocks);
		if (LEXER_DEBUG_FILE)
			printf("--- %s ---\n", block.label);
		if(t_line != NULL)
			free(t_line);

		if (eof)
			break;
	}
	fclose(asm_f);
	if (!check_for_start(out_blocks)) {
		printf("[PANIC] .start label is not found\n");
		// TODO:
		// since i've reworked blocks architecture, it's not possible to free_blocks here
		// without stack smashing, Sadge
		// free_blocks(block);
		exit(1);
	}
}

void print_code_blocks(code_blocks *_blocks) {
	printf("code block count: %hu\n", _blocks->count);
	for (u16 i = 0; i < _blocks->count; ++i) {
		printf("block[%hu] %s\n", i, _blocks->block[i].label);
		for (u16 j = 0; j < _blocks->block[i].ins.count; ++j) {
			printf("INS[%hu] %s\n", j, _blocks->block[i].ins.cmds[j].ins.token);
		}
		printf("\n");
	}
}

/* needs rewriting */
// void print_ins_set() {
// 	for (u16 i = 0; i < ins_pool.count; ++i) {
// 		printf("[%hu]: %s\n\n", i, ins_to_str(ins_pool.cmds[i].ins));
// 		printf("[%hu]: val1 %s\n", i, ins_pool.cmds[i].val1_type == T_VAL1_REG ? "REGISTRY" : "NUMBER");
// 		if (ins_pool.cmds[i].val1_type == T_VAL1_REG)
// 			printf("[%hu]: val1 %s\n\n", i, reg_to_str(ins_pool.cmds[i].val1.reg));
// 		else
// 			printf("[%hu]: val1 %d\n\n", i, ins_pool.cmds[i].val1.num);
//
// 		printf("[%hu]: val2 %s\n", i, ins_pool.cmds[i].val2_type == T_VAL2_REG ? "REGISTRY" : "NUMBER");
// 		if (ins_pool.cmds[i].val2_type == T_VAL2_REG)
// 			printf("[%hu]: val2 %s\n\n", i, reg_to_str(ins_pool.cmds[i].val2.reg));
// 		else
// 			printf("[%hu]: val2 %d\n\n", i, ins_pool.cmds[i].val2.num);
//
// 		printf("----------------\n");
// 	}
// 	printf("Total instruction count: %hu\n", ins_pool.count);
// }

#ifdef VM_BUILD
void start_interpreter(cpu *_cpu, code_blocks *_blocks) {
	assert(_blocks->count != 0 && "No code blocks provided");	
	if (LEXER_DEBUG_INSTRUCTIONS)
		ins_dbg_print();
	for (u16 i = 0; i < _blocks->count; ++i) {
		if (strcmp(_blocks->block[i].label, "start") == 0) {
			_cpu->ip.block = &_blocks->block[i];
			_cpu->ip.ins = 0;
			if (execute_interpreter(_cpu, _blocks) == -1)
				printf("[PANIC] HALTING\n");
			break;
		}
	}
	free_blocks(_blocks);
	if (LEXER_DEBUG_STACK)
		print_stack(_cpu);
}
#endif
#endif
