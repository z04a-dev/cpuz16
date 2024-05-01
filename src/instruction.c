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

int DEBUG_PRINT = 0;

int execute_code(cpu *_cpu, code_blocks *_code_blocks, instruction_pointer *_ip); 

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

static int ins_jmp(cpu *_cpu, cmd _cmd, code_blocks *_code_blocks, instruction_pointer *_ip) {
	if (DEBUG_PRINT) {
		printf("JMP\n");
	}
	(void)_cpu;
	if (_cmd.val1_type != T_VAL1_LABEL || _cmd.val2_type != T_VAL2_LABEL) {
		printf("[PANIC] wrong variable type while jumping\n");
	}
	for (u16 i = 0; i < _code_blocks->count; ++i) {
		assert(strcmp(_cmd.val1.label, "start") && "you cant jump to .start loser");
		if (strcmp(_cmd.val1.label, _code_blocks->block[i].label) == 0) {
			if (_code_blocks->block[i].ins.count != 0) {
				_ip->block = &_code_blocks->block[i];
				_ip->ins = 0;
			}
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
			_cpu->rax++;
			break;
		case REG_RBX:
			if (DEBUG_PRINT)
				printf("INC RBX\n");
			_cpu->rbx++;
			break;
		case REG_RDX:
			if (DEBUG_PRINT)
				printf("INC RDX\n");
			_cpu->rdx++;
			break;
		case REG_A1:
			if (DEBUG_PRINT)
				printf("INC A1\n");
			_cpu->a1++;
			break;
		case REG_A2:
			if (DEBUG_PRINT)
				printf("INC A2\n");
			_cpu->a2++;
			break;
		case REG_A3:
			if (DEBUG_PRINT)
				printf("INC A3\n");
			_cpu->a3++;
			break;
		case REG_INS:
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

int execute_instruction(cpu *_cpu, cmd *_cmd, code_blocks *_code_blocks, instruction_pointer *_ip) {
	switch(_cmd->ins) {
		case INS_NOP:
			ins_nop(_cpu); 
			break;
		case INS_JMP:
			if (ins_jmp(_cpu, *_cmd, _code_blocks, _ip) == -1) 
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
			printf("[WARNING] something went wrong, we executed end; instruction during runtime.\n");
			break;
	}
	return 0;
}


int execute_code(cpu *_cpu, code_blocks *_code_blocks, instruction_pointer *_ip) {
	while (execute_instruction(_cpu, &_ip->block->ins.cmds[_ip->ins], _code_blocks, _ip) != -1) {
		ins_inc(_cpu, REG_INS); /* instruction counter + 1 */
		_ip->ins++;
		// printf("CPU INS: %hu\n", _cpu->ins);
		// printf("CPU RBX: %hu\n", _cpu->rbx);
		// printf("CPU RAX: %hu\n", _cpu->rax);
	}
	return -1;
}

#endif
