#include <string.h>
#ifndef u16
#define u16 unsigned short
#include "stdio.h"
#endif

#ifndef _INS_IMPL_
#define _INS_IMPL_
#include "struct.h"
#include <assert.h>
#include <stdlib.h>

#define u16_MAX 65535

/* god i hate stack overflow */
#define MAX_RECURSIVE_DEPTH 10000 

int DEBUG_PRINT = 0;

int execute_block(cpu *_cpu, code_blocks *_code_blocks, code_block *target, u16 *depth); 

static u16 get_val2_from_cmd(cpu *_cpu, cmd _cmd) {
	if (_cmd.val2_type == T_VAL2_U16)
		return _cmd.val2.num;
	else {
		switch(_cmd.val2.reg) {
			case REG_RAX:
				return _cpu->rax;
			case REG_RBX:
				return _cpu->rbx;
			case REG_RDX:
				return _cpu->rdx;
			case REG_A1:
				return _cpu->a1;
			case REG_A2:
				return _cpu->a2;
			case REG_A3:
				return _cpu->a3;
			case REG_INS:
				return _cpu->ins;
		}
	}
	return 0;
}

static void ins_nop(cpu *_cpu, ...) {
	(void)_cpu;
	if (DEBUG_PRINT) {
		printf("NOP\n");
	}
}

static void ins_add(cpu *_cpu, cmd _cmd, ...) {
	u16 value = get_val2_from_cmd(_cpu, _cmd);

	switch(_cmd.val1.reg) {
		case REG_RAX:
			if(DEBUG_PRINT)
				printf("ADD RAX %hu\n", value);
			_cpu->rax += value;
			break;
		case REG_RBX:
			if(DEBUG_PRINT)
				printf("ADD RBX %hu\n", value);
			_cpu->rbx += value;
			break;
		case REG_RDX:
			if(DEBUG_PRINT)
				printf("ADD RDX %hu\n", value);
			_cpu->rdx += value;
			break;
		case REG_A1:
			if(DEBUG_PRINT)
				printf("ADD A1 %hu\n", value);
			_cpu->a1 += value;
			break;
		case REG_A2:
			if(DEBUG_PRINT)
				printf("ADD A2 %hu\n", value);
			_cpu->a2 += value;
			break;
		case REG_A3:
			if(DEBUG_PRINT)
				printf("ADD A3 %hu\n", value);
			_cpu->a3 += value;
			break;
		case REG_INS:
			if(DEBUG_PRINT)
				printf("ADD INS %hu\n", value);
			_cpu->ins += value;
			break;
	}
}

static void ins_sub(cpu *_cpu, cmd _cmd) {

	u16 value = get_val2_from_cmd(_cpu, _cmd);

	switch(_cmd.val1.reg) {
		case REG_RAX:
			if(DEBUG_PRINT)
				printf("SUB RAX %hu\n", value);
			_cpu->rax -= value;
			break;
		case REG_RBX:
			if(DEBUG_PRINT)
				printf("SUB RBX %hu\n", value);
			_cpu->rbx -= value;
			break;
		case REG_RDX:
			if(DEBUG_PRINT)
				printf("SUB RDX %hu\n", value);
			_cpu->rdx -= value;
			break;
		case REG_A1:
			if(DEBUG_PRINT)
				printf("SUB A1 %hu\n", value);
			_cpu->a1 -= value;
			break;
		case REG_A2:
			if(DEBUG_PRINT)
				printf("SUB A2 %hu\n", value);
			_cpu->a2 -= value;
			break;
		case REG_A3:
			if(DEBUG_PRINT)
				printf("SUB A3 %hu\n", value);
			_cpu->a3 -= value;
			break;
		case REG_INS:
			if(DEBUG_PRINT)
				printf("SUB INS %hu\n", value);
			_cpu->ins -= value;
			break;
	}
}

static int ins_jmp(cpu *_cpu, cmd _cmd, code_blocks *_code_blocks, u16 *depth) {
	if (*depth >= MAX_RECURSIVE_DEPTH) {
		printf("[PANIC] jump depth is too high\n");
		printf("[PANIC] risk of stack overflow\n");
		return -1;
	}
	if (DEBUG_PRINT) {
		printf("JMP\n");
	}
	(void)_cpu;
	if (_cmd.val1_type != T_VAL1_LABEL || _cmd.val2_type != T_VAL2_LABEL) {
		printf("[PANIC] wrong variable type while jumping\n");
	}
	*depth += 1;
	for (u16 i = 0; i < _code_blocks->count; ++i) {
		assert(strcmp(_cmd.val1.label, "start") && "you cant jump to .start loser");
		if (strcmp(_cmd.val1.label, _code_blocks->block[i].label) == 0) {
			if (execute_block(_cpu, _code_blocks, &_code_blocks->block[i], depth) == -1)
				return -1;
			break;
		}
	}
	return 0;
} 

static void ins_inc(cpu *_cpu, enum REGISTRY reg) {
	switch(reg) {
		case REG_RAX:
			if (DEBUG_PRINT)
				printf("INC RAX\n");
			if (_cpu->rax == u16_MAX)
				_cpu->rax = 0;
			_cpu->rax++;
			break;
		case REG_RBX:
			if (DEBUG_PRINT)
				printf("INC RBX\n");
			if (_cpu->rbx == u16_MAX)
				_cpu->rbx = 0;
			_cpu->rbx++;
			break;
		case REG_RDX:
			if (DEBUG_PRINT)
				printf("INC RDX\n");
			if (_cpu->rdx == u16_MAX)
				_cpu->rdx = 0;
			_cpu->rdx++;
			break;
		case REG_A1:
			if (DEBUG_PRINT)
				printf("INC A1\n");
			if (_cpu->a1 == u16_MAX)
				_cpu->a1 = 0;
			_cpu->a1++;
			break;
		case REG_A2:
			if (DEBUG_PRINT)
				printf("INC A2\n");
			if (_cpu->a2 == u16_MAX)
				_cpu->a2 = 0;
			_cpu->a2++;
			break;
		case REG_A3:
			if (DEBUG_PRINT)
				printf("INC A3\n");
			if (_cpu->a3 == u16_MAX)
				_cpu->a3 = 0;
			_cpu->a3++;
			break;
		case REG_INS:
			if (_cpu->ins == 65535) {
				_cpu->ins = 0;
			}
			_cpu->ins++;
			break;
	}
}

static void ins_dec(cpu *_cpu, enum REGISTRY reg) {
	switch(reg) {
		case REG_RAX:
			if (DEBUG_PRINT)
				printf("DEC RAX\n");
			_cpu->rax--;
			break;
		case REG_RBX:
			if (DEBUG_PRINT)
				printf("DEC RBX\n");
			_cpu->rbx--;
			break;
		case REG_RDX:
			if (DEBUG_PRINT)
				printf("DEC RDX\n");
			_cpu->rdx--;
			break;
		case REG_A1:
			if (DEBUG_PRINT)
				printf("DEC A1\n");
			_cpu->a1--;
			break;
		case REG_A2:
			if (DEBUG_PRINT)
				printf("DEC A2\n");
			_cpu->a2--;
			break;
		case REG_A3:
			if (DEBUG_PRINT)
				printf("DEC A3\n");
			_cpu->a3--;
			break;
		case REG_INS:
			if (DEBUG_PRINT)
				printf("DEC INS\n");
			assert(0 && "You can't decrement instruction counter.");
			break;
	}
}

static void ins_halt(cpu *_cpu, ...) {
	(void)_cpu;
	printf("[PANIC] MET HALT\n");
}

void ins_dbg_print() { /* turn on/off INS printing */
	if (DEBUG_PRINT) {
		printf("PRINT DEBUG: 0\n");
		DEBUG_PRINT = 0;
	}
	else {
		printf("PRINT DEBUG: 1\n");
		DEBUG_PRINT = 1;
	}
}

cmd IC_add = {.ins = INS_INC,
	.val1_type = T_VAL1_REG,
	.val1.reg = REG_INS};


int execute_instruction(cpu *_cpu, cmd *_cmd, code_blocks *_code_blocks, u16 *depth) {
	switch(_cmd->ins) {
		case INS_NOP:
			ins_nop(_cpu); 
			break;
		case INS_JMP:
			if (ins_jmp(_cpu, *_cmd, _code_blocks, depth) == -1) 
				return -1;
			break;
		case INS_ADD:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_ADD\n");
				exit(1);
			}
			ins_add(_cpu, *_cmd);
			break;
		case INS_SUB:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_SUB\n");
				exit(1);
			}
			ins_sub(_cpu, *_cmd);
			break;
		case INS_INC:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_INC\n");
				exit(1);
			}
			ins_inc(_cpu, _cmd->val1.reg); 
			break;
		case INS_DEC:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_DEC\n");
				exit(1);
			}
			ins_dec(_cpu, _cmd->val1.reg); 
			break;
		case INS_HALT:
			ins_halt(_cpu); 
			return -1;
			break;
		case INS_END:
			printf("[WARNING] something went wrong, we processes and executed end; instruction.\n");
			break;
	}
	return 0;
}

int jumps = 0;

int execute_block(cpu *_cpu, code_blocks *_code_blocks, code_block *target, u16 *depth) {
	if (DEBUG_PRINT) {
		printf("CPU INS: %hu\n", _cpu->ins);
		printf("CPU RBX: %hu\n", _cpu->rbx);
		printf("CPU RAX: %hu\n", _cpu->rax);
		printf("JUMPS: %d\n", ++jumps);
	}
	u16 current_depth = *depth;
	for (u16 ins = 0; ins < target->ins.count; ++ins) {
		execute_instruction(_cpu, &IC_add, _code_blocks, depth);
		if (execute_instruction(_cpu, &target->ins.cmds[ins], _code_blocks, depth) == -1) {
			*depth = current_depth;
			return -1;	
		}
	}
	*depth = current_depth;
	return 0;
}

#endif
