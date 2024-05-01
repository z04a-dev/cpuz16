#include "instruction.h"

#ifndef _LEXER_IMPL_
#define _LEXER_IMPL_
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define LEXER_DEBUG_FILE false // debug at parsing
#define LEXER_DEBUG_INSTRUCTIONS false // enable execute_instructions debug function

#define REG_LEN 7
// TODO i guess i dont need REG_INS here
// maybe i do...
char *REG_NAMES[REG_LEN] = {"rax",
	"rbx",
	"rdx",
	"a1",
	"a2",
	"a3",
	"ins"};


int COUNT = -1; // TODO DELETE

// instruction_pool ins_pool = {.capacity = -1};
code_blocks blocks = {.capacity = -1};

void free_blocks() {
	printf("[VM] freeing blocks\n");
	if (blocks.count != 0) {
		for (u16 i = 0; i < blocks.count; ++i) {
			if (blocks.block[i].ins.count != 0) {
				free(blocks.block[i].ins.cmds);
			}
		}
		free(blocks.block);
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

static enum INSTRUCTION recognize_ins(char *ins) {
	if (strcmp(ins, "add") == 0)
		return INS_ADD;
	if (strcmp(ins, "sub") == 0)
		return INS_SUB;
	if (strcmp(ins, "inc") == 0)
		return INS_INC;
	if (strcmp(ins, "dec") == 0)
		return INS_DEC;
	if (strcmp(ins, "jmp") == 0)
		return INS_JMP;
	if (strcmp(ins, "halt") == 0)
		return INS_HALT;
	if (strcmp(ins, "nop") == 0)
		return INS_NOP;
	if (strcmp(ins, "end") == 0)
		return INS_END;
	return -1;
}

static char *ins_to_str(enum INSTRUCTION ins) {
	switch (ins) {
		case INS_NOP:
			return "NOP";
			break;
		case INS_ADD:
			return "ADD";
			break;
		case INS_SUB:
			return "SUB";
			break;
		case INS_INC:
			return "INC";
			break;
		case INS_DEC:
			return "DEC";
			break;
		case INS_HALT:
			return "HALT";
			break;
		case INS_JMP:
			return "JMP";
			break;
		case INS_END:
			return "END";
			break;
	}
	return "";
}

static char *reg_to_str(enum REGISTRY reg) {
	switch (reg) {
		case REG_RAX:
			return "rax";
			break;
		case REG_RBX:
			return "rbx";
			break;
		case REG_RDX:
			return "rdx";
			break;
		case REG_A1:
			return "a1";
			break;
		case REG_A2:
			return "a2";
			break;
		case REG_A3:
			return "a3";
			break;
		case REG_INS:
			return "ins";
			break;
	}
	return "";
}

static void debug_print_cmd(cmd *_cmd) {
	char *_v1;
	if (_cmd->val1_type == T_VAL1_U16)
		asprintf(&_v1, "%hu", _cmd->val1.num);
	else
		asprintf(&_v1, "%s", reg_to_str(_cmd->val1.reg));

	char *_v2;
	if (_cmd->val2_type == T_VAL2_U16)
		asprintf(&_v2, "%hu", _cmd->val2.num);
	else
		asprintf(&_v2, "%s", reg_to_str(_cmd->val2.reg));

	printf("[DEBUG] cmd: %s val1: %s val2: %s\n", 
			_cmd->ins == INS_ADD ? "ADD" :
			_cmd->ins == INS_SUB ? "SUB" : 
			_cmd->ins == INS_INC ? "INC" :
			_cmd->ins == INS_DEC ? "DEC" : 
			_cmd->ins == INS_JMP ? "JMP" :
			_cmd->ins == INS_NOP ? "NOP" :
			"HALT", _v1, _v2);
}

static bool is_token_registry(char *token, int *reg) {
	for (int i = 0; i < REG_LEN; ++i)
		if (strcmp(token, REG_NAMES[i]) == 0) {
			*reg = i;
			return true;
		}
	return false;
}

static void populate_code_blocks(code_block _block) {
	if (blocks.count == 65535) {
		printf("[PANIC] too much code blocks (must be less than 65535)");
		exit(1);
	}
	blocks.count++;
	blocks.block = realloc(blocks.block, blocks.count * sizeof(code_block));
	blocks.block[blocks.count - 1] = _block;
}


static cmd tokenize_str(char *str) {
	cmd _cmd = {0};
	char *DEBUG_STR;
	asprintf(&DEBUG_STR, "%s", str);


	char *token;
	int count = 0;
	char *p_clean;
	do {
		if (count == 0)
			token = strtok(str, " ");
		else
			token = strtok(NULL, " ");
		if (token == NULL)
			break;

		p_clean = strrchr(token, ',');
		if (p_clean == NULL) {
			p_clean = strrchr(token, ';');
		}
		if (p_clean != NULL) {
			*p_clean = '\0';
		}
		if (count == 0) {
			_cmd.ins = recognize_ins(token); 
			if (strcmp(token, "jmp") == 0) {
				_cmd.val1_type = T_VAL1_LABEL;
				_cmd.val2_type = T_VAL2_LABEL;
			}
		}
		else if (count == 1) {
			int reg = 0;
			if (_cmd.val1_type == T_VAL1_LABEL)
				asprintf(&_cmd.val1.label, "%s", token);
			else if (is_token_registry(token, &reg)) {
				assert(!(_cmd.ins == INS_INC && reg == REG_INS) && "You can't increment IC manually.");

				_cmd.val1_type = T_VAL1_REG;
				_cmd.val1.reg = reg; 
			} else {
				_cmd.val1_type = T_VAL1_U16;
				_cmd.val1.num = (u16)atoi(token); 
			}
		}
		else if (count == 2) {
			int reg = 0;
			if (_cmd.val2_type == T_VAL2_LABEL)
				asprintf(&_cmd.val2.label, "%s", token);
			else if (is_token_registry(token, &reg)) {
				_cmd.val2_type = T_VAL2_REG;
				_cmd.val2.reg = reg; 
			} else {
				_cmd.val2_type = T_VAL2_U16;
				_cmd.val2.num = (u16)atoi(token); 
			}
			break;
		}

		count++;
		
	} while (token != NULL);
	if ((int)_cmd.ins == -1) {
		printf("SIGILL: lllegal instruction: <%s>\n", DEBUG_STR);
		exit(1);
	}

	if (token != NULL)
		free(token);

	// printf("%s\n", 
	// 		_cmd.ins == INS_ADD ? "ADD" :
	// 		_cmd.ins == INS_SUB ? "SUB" : 
	// 		_cmd.ins == INS_INC ? "INC" :
	// 		_cmd.ins == INS_DEC ? "DEC" : 
	// 		_cmd.ins == INS_JMP ? "JMP" :
	// 		_cmd.ins == INS_NOP ? "NOP" :
	// 		"HALT");
	
	if (LEXER_DEBUG_FILE) {
		printf("line: %s\n", DEBUG_STR);
		debug_print_cmd(&_cmd);
		printf("\n");
	}

	if (DEBUG_STR != NULL)
		free(DEBUG_STR);
	return _cmd; 
}

static bool process_str(char *str, code_block *block) {
	COUNT++;
	char *end_ptr = strrchr(str, '\n');
	if (end_ptr != NULL) {
		*end_ptr = '\0';
	}
	if (strlen(str) == 0)
		return true;
	str = remove_start_whitespaces(str);
	if (is_comment(str))
		return true;
	cmd _cmd = tokenize_str(str);
	if (_cmd.ins == INS_END) {
		return false;
	}
	add_ins_to_pool(&block->ins, _cmd);
	return true;
}

static bool check_for_start() {
	for (u16 i = 0; i < blocks.count; ++i) {
		if (strcmp(blocks.block[i].label, "start") == 0)
			return true;
	}
	return false;
}

void start_lexer(char *asm_file) {
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
		} while (process_str(line, &block));
		populate_code_blocks(block);
		if(t_line != NULL)
			free(t_line);

		if (eof)
			break;
	}
	fclose(asm_f);
	if (!check_for_start()) {
		printf("[PANIC] .start label is not found\n");
		free_blocks();
		exit(1);
	}
}

void print_code_blocks() {
	printf("code block count: %hu\n", blocks.count);
	for (u16 i = 0; i < blocks.count; ++i) {
		printf("block[%hu] %s\n", i, blocks.block[i].label);
		for (u16 j = 0; j < blocks.block[i].ins.count; ++j) {
			printf("INS[%hu] %s\n", j, ins_to_str(blocks.block[i].ins.cmds[j].ins));
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


void start_executing(cpu *_cpu) {
	assert(blocks.count != 0 && "No code blocks provided");	
	if (LEXER_DEBUG_INSTRUCTIONS)
		ins_dbg_print();
	u16 depth = 0;
	for (u16 i = 0; i < blocks.count; ++i) {
		if (strcmp(blocks.block[i].label, "start") == 0) {
			if (execute_block(_cpu, &blocks, &blocks.block[i], &depth) == -1)
				printf("[PANIC] HALTING\n");
			break;
		}
	}
	free_blocks();
}
#endif
