#ifndef u16
#define u16 unsigned short
#endif

#ifndef _INS_IMPL_
#define _INS_IMPL_
#include "struct.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define u16_MAX 65535

int DEBUG_PRINT = 0;

int execute_code(cpu *_cpu, code_blocks *_code_blocks); 

u16 get_registry_value(cpu *_cpu, enum REGISTRY reg) {
	switch (reg) {
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
	return 0;
}

static void put_value_in_reg(cpu *_cpu, enum REGISTRY reg, u16 value) {
	switch (reg) {
        case REG_RAX:
			_cpu->rax = value;
			break;
        case REG_RBX:
			_cpu->rbx = value;
			break;
        case REG_RDX:
			_cpu->rdx = value;
			break;
        case REG_A1:
			_cpu->a1 = value;
			break;
        case REG_A2:
			_cpu->a2 = value;
			break;
        case REG_A3:
			_cpu->a3 = value;
			break;
        case REG_INS:
			_cpu->ins = value;
			break;
	}
}

static u16 get_val2_from_cmd(cpu *_cpu, cmd _cmd) {
	if (_cmd.val2_type == T_VAL2_U16)
		return _cmd.val2.num;
	else if (_cmd.val2_type == T_VAL2_REG)
		return get_registry_value(_cpu, _cmd.val2.reg);
	return 0;
}

char *ins_to_str(enum INSTRUCTION ins) {
	switch (ins) {
		case INS_MOV:
			return "MOV";
			break;
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
		case INS_RET:
			return "RET";
			break;
		case INS_END:
			return "END";
			break;
		case INS_PUSH:
			return "PUSH";
			break;
		case INS_POP:
			return "POP";
			break;
		case INS_OR:
			return "OR";
			break;
		case INS_AND:
			return "AND";
			break;
		case INS_XOR:
			return "XOR";
			break;
		}
	return "";
}

char *reg_to_str(enum REGISTRY reg) {
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

int push_stack(cpu *_cpu, u16 *_value) {
	if(_cpu->stack_pointer == &_cpu->stack_value[STACK_SIZE]) {
		printf("[PANIC] stack overflow!\n");
		return -1;
	}
	*_cpu->stack_pointer = *_value;
	_cpu->stack_pointer++; 
	return 0;
}

int pop_stack(cpu *_cpu, u16 *_value) {
	if(_cpu->stack_pointer == _cpu->stack_value) {
		printf("[PANIC] stack underflow!\n");
		return -1;
	}
	_cpu->stack_pointer--;
	*_value = *_cpu->stack_pointer;
	*_cpu->stack_pointer = 0;
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

static int ins_jmp(cpu *_cpu, cmd _cmd, code_blocks *_code_blocks) {
	if (DEBUG_PRINT) {
		printf("JMP");
	}
	if (_cmd.val1_type != T_VAL1_LABEL || _cmd.val2_type != T_VAL2_LABEL) {
		printf("[PANIC] wrong variable type while jumping\n");
	}
	for (u16 i = 0; i < _code_blocks->count; ++i) {
		assert(strcmp(_cmd.val1.label, "start") && "you cant jump to .start loser");
		if (strcmp(_cmd.val1.label, _code_blocks->block[i].label) == 0) {
			if (_code_blocks->block[i].ins.count > 0) {
				_cpu->rp.ins = _cpu->ip.ins + 1;
				_cpu->rp.block = _cpu->ip.block;
				_cpu->ip.block = &_code_blocks->block[i];
				_cpu->ip.ins = 0;
				if (DEBUG_PRINT) {
					printf(" RP: %s:%d", _cpu->rp.block->label, _cpu->rp.ins);
					printf(" IP: %s:%d\n", _cpu->ip.block->label, _cpu->ip.ins);
				}
			}
			break;
		}
	}
	return 0;
} 

static void ins_ret(cpu *_cpu) {
	if (DEBUG_PRINT) 
		printf("RET\n");
	assert(_cpu->rp.block != NULL && "Return Pointer is nil");
	_cpu->ip = _cpu->rp;
	_cpu->rp.block = NULL;
	_cpu->rp.ins = 0;
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

void print_stack(cpu *_cpu) {
	for (u16 i = 0; i < STACK_SIZE; ++i) {
		if (_cpu->stack_pointer == &_cpu->stack_value[i])
			printf("[%hu]: %hu <--------\n", i, _cpu->stack_value[i]);
		else
			printf("[%hu]: %hu\n", i, _cpu->stack_value[i]);
	}
}

static int ins_push(cpu *_cpu, cmd *_cmd) {
	if (DEBUG_PRINT)
		printf("PUSH %s\n", reg_to_str(_cmd->val1.reg));
	u16 reg_value = get_registry_value(_cpu, _cmd->val1.reg);
	if (push_stack(_cpu, &reg_value) == -1)
		return -1;
	return 0;
}

static int ins_pop(cpu *_cpu, cmd *_cmd) {
	if (DEBUG_PRINT)
		printf("POP %s\n", reg_to_str(_cmd->val1.reg));
	u16 reg_value; 
	if (pop_stack(_cpu, &reg_value) == -1)
		return -1;
	put_value_in_reg(_cpu, _cmd->val1.reg, reg_value);
	return 0;
}

static void ins_or(cpu *_cpu, cmd *_cmd) {
	if (DEBUG_PRINT)
		printf("OR %s, %hu\n", reg_to_str(_cmd->val1.reg), get_val2_from_cmd(_cpu, *_cmd));
	u16 val1 = get_registry_value(_cpu, _cmd->val1.reg);
	u16 val2 = get_val2_from_cmd(_cpu, *_cmd);
	put_value_in_reg(_cpu, _cmd->val1.reg, val1 | val2);
}

static void ins_and(cpu *_cpu, cmd *_cmd) {
	if (DEBUG_PRINT)
		printf("AND %s, %hu\n", reg_to_str(_cmd->val1.reg), get_val2_from_cmd(_cpu, *_cmd));
	u16 val1 = get_registry_value(_cpu, _cmd->val1.reg);
	u16 val2 = get_val2_from_cmd(_cpu, *_cmd);
	put_value_in_reg(_cpu, _cmd->val1.reg, val1 & val2);
}

static void ins_xor(cpu *_cpu, cmd *_cmd) {
	if (DEBUG_PRINT)
		printf("XOR %s, %hu\n", reg_to_str(_cmd->val1.reg), get_val2_from_cmd(_cpu, *_cmd));
	u16 val1 = get_registry_value(_cpu, _cmd->val1.reg);
	u16 val2 = get_val2_from_cmd(_cpu, *_cmd);
	put_value_in_reg(_cpu, _cmd->val1.reg, val1 ^ val2);
}

static void ins_halt(cpu *_cpu, ...) {
	(void)_cpu;
	printf("[PANIC] MET HALT\n");
}

// most important one!
static void ins_mov(cpu *_cpu, cmd *_cmd) {
	if (_cmd->val1_type == T_VAL1_REG) {
		if (DEBUG_PRINT) 
			printf("MOV TO REG %s, %hu\n", reg_to_str(_cmd->val1.reg),
					_cmd->val2_type == T_VAL2_ADDRESS ? _cmd->val2.num : get_val2_from_cmd(_cpu, *_cmd));
		switch (_cmd->val2_type) {
			case T_VAL2_REG: 
				put_value_in_reg(_cpu, _cmd->val1.reg, get_val2_from_cmd(_cpu, *_cmd));
				break;
			case T_VAL2_U16: 
				put_value_in_reg(_cpu, _cmd->val1.reg, get_val2_from_cmd(_cpu, *_cmd));
				break;
			case T_VAL2_ADDRESS:
				assert (_cmd->val2.num < RAM_SIZE - STACK_SIZE + 1 && "mov address can't point to stack");
				put_value_in_reg(_cpu, _cmd->val1.reg, _cpu->ram.cells[_cmd->val2.num]);
				break;
			default:
				assert(0 && "well we shouldn't be here (INS_MOV)");
				break;
		}
	} else if (_cmd->val1_type == T_VAL1_ADDRESS) {
		if (DEBUG_PRINT) 
			printf("MOV TO ADDR %hu, %hu\n", _cmd->val1.num, 
					_cmd->val2_type == T_VAL2_ADDRESS ? _cmd->val2.num : get_val2_from_cmd(_cpu, *_cmd));
		switch (_cmd->val2_type) {
			case T_VAL2_REG: 
				assert (_cmd->val1.num < RAM_SIZE - STACK_SIZE + 1 && "mov address can't point to stack");
				_cpu->ram.cells[_cmd->val1.num] = get_val2_from_cmd(_cpu, *_cmd);
				break;
			case T_VAL2_U16: 
				assert (_cmd->val1.num < RAM_SIZE - STACK_SIZE + 1 && "mov address can't point to stack");
				_cpu->ram.cells[_cmd->val1.num] = get_val2_from_cmd(_cpu, *_cmd);
				break;
			case T_VAL2_ADDRESS:
				assert (_cmd->val2.num < RAM_SIZE - STACK_SIZE + 1 && "mov address can't point to stack");
				_cpu->ram.cells[_cmd->val1.num] = _cpu->ram.cells[_cmd->val2.num];
				break;
			default:
				assert(0 && "well we shouldn't be here (INS_MOV)");
				break;
		}
	}
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

int execute_instruction(cpu *_cpu, cmd *_cmd, code_blocks *_code_blocks) {
	switch(_cmd->ins) {
		case INS_MOV:
			assert(_cmd->val1_type != T_VAL1_U16 && "[ERROR] You can't mov data to num!");
			ins_mov(_cpu, _cmd);
			break;
		case INS_NOP:
			ins_nop(_cpu); 
			break;
		case INS_JMP:
			if (ins_jmp(_cpu, *_cmd, _code_blocks) == -1) 
				return -1;
			return 0;
			break;
		case INS_RET:
			ins_ret(_cpu);
			return 0;
			break;
		case INS_ADD:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_ADD\n");
				return -1;
			}
			ins_add(_cpu, *_cmd);
			break;
		case INS_SUB:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_SUB\n");
				return -1;
			}
			ins_sub(_cpu, *_cmd);
			break;
		case INS_INC:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_INC\n");
				return -1;
			}
			ins_inc(_cpu, _cmd->val1.reg); 
			break;
		case INS_DEC:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_DEC\n");
				return -1;
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
		case INS_PUSH:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_PUSH\n");
				return -1;
			}
			ins_push(_cpu, _cmd);
			break;
		case INS_POP:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_POP\n");
				return -1;
			}
			ins_pop(_cpu, _cmd);
			break;
		case INS_OR:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_OR\n");
				return -1;
			}
			ins_or(_cpu, _cmd);
			break;
		case INS_AND:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_AND\n");
				return -1;
			}
			ins_and(_cpu, _cmd);
			break;
		case INS_XOR:
			if(_cmd->val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_XOR\n");
				return -1;
			}
			ins_xor(_cpu, _cmd);
			break;
	}
			_cpu->ip.ins++;
	return 0;
}

int execute_code(cpu *_cpu, code_blocks *_code_blocks) {
	while (execute_instruction(_cpu, &_cpu->ip.block->ins.cmds[_cpu->ip.ins], _code_blocks) != -1 && 
			_cpu->ip.ins < _cpu->ip.block->ins.count) {
		ins_inc(_cpu, REG_INS); /* instruction counter + 1 */
		// printf("CPU INS: %hu\n", _cpu->ins);
		// printf("CPU RBX: %hu\n", _cpu->rbx);
		// printf("CPU RAX: %hu\n", _cpu->rax);
	}
	// print_stack(_cpu);
	return -1;
}

#endif
