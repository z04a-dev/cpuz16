#ifndef u16
#define u16 unsigned short
#include "stdio.h"
#endif

#ifndef _INS_IMPL_
#define _INS_IMPL_
#include "struct.h"
#include <assert.h>
#include <stdlib.h>

int DEBUG_PRINT = 0;

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

static void ins_jmp(cpu *_cpu, ...) {
	if (DEBUG_PRINT) {
		printf("JMP\n");
	}
	(void)_cpu;
} /* alpha (NOT USED) (TODO) */

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

int execute_instruction(cpu *_cpu, cmd _cmd) {
	switch(_cmd.ins) {
		case INS_NOP:
			ins_nop(_cpu); 
			break;
		case INS_JMP:
			ins_jmp(_cpu); 
			break;
		case INS_ADD:
			if(_cmd.val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_ADD\n");
				exit(1);
			}
			ins_add(_cpu, _cmd);
			break;
		case INS_SUB:
			if(_cmd.val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_SUB\n");
				exit(1);
			}
			ins_sub(_cpu, _cmd);
			break;
		case INS_INC:
			if(_cmd.val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_INC\n");
				exit(1);
			}
			ins_inc(_cpu, _cmd.val1.reg); 
			break;
		case INS_DEC:
			if(_cmd.val1_type == T_VAL1_U16) {
				printf("[ERROR] Illegal values provided to INS_DEC\n");
				exit(1);
			}
			ins_dec(_cpu, _cmd.val1.reg); 
			break;
		case INS_HALT:
			ins_halt(_cpu); 
			return -1;
			break;
	}
	return 0;
}

#endif
