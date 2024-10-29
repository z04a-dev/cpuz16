#ifndef u16
#define u16 unsigned short
#endif

#define _STRUCT_IMPL

#include <stdbool.h>
#define STACK_SIZE 256 
#define RAM_SIZE 32767

typedef struct define {
	char *name;
	enum{ T_DEF_NULL, T_DEF_IMM, T_DEF_ASCII, T_DEF_DATA} def_type;
	union {
		u16 imm;
		char *ascii;
		u16 *data;
	} value;
	u16 data_size;
} define;

typedef struct define_block {
	define *def;
	unsigned int count;
} define_block;

typedef struct instruction {
	u16 opcode;
	const char *token;
} ins;

typedef struct instruction_set {
	ins *ins;
	unsigned int count;
} instruction_set;

#define CLITERAL(type) (type) /* yoinked from raylib */

#define REGISTRY_COUNT 7

typedef struct {
	ins ins;
	enum{ T_VAL1_NULL, T_VAL1_U16, T_VAL1_REG, T_VAL1_LABEL} val1_type;
	union {
		u16 num;
		u16 reg;
		char *label;
	} val1;
	enum{ T_VAL2_NULL, T_VAL2_U16, T_VAL2_REG, T_VAL2_LABEL} val2_type;
	union {
		u16 num;
		u16 reg;
		char *label;
	} val2;
	enum{ T_VAL3_NULL, T_VAL3_U16, T_VAL3_REG, T_VAL3_LABEL} val3_type;
	union {
		u16 num;
		u16 reg;
		char *label;
	} val3;
} cmd;

typedef struct {
	cmd *cmds;
	u16 count;
	u16 capacity;
} instruction_pool;

typedef struct {
	instruction_pool ins;
	char *label;
} code_block;

typedef struct {
	code_block *block;
	u16 count;
	u16 capacity;
} code_blocks;

typedef struct {
	code_block *block;
	u16 ins;
} instruction_pointer;

typedef struct {
	u16 *cells;
	u16 capacity;
} ram;

#define return_pointer instruction_pointer

typedef enum {
	VM_BINARY,
	VM_INTERPRETER
} state;

typedef struct cpu {
	state state;
	ram ram;
	instruction_pointer ip; // for interpreting .asm
	return_pointer rp; // for interpreting .asm
	u16 *stack_value;
	u16 *stack_pointer;
	u16 rax;
	u16 rbx;
	u16 rdx;
	u16 a1;
	u16 a2;
	u16 a3;
	// TODO
	// Since i'm adding bytecode execution support, this registry will be used as PC,
	// so i need to differentiate between debug IC (for interpreter), and PC (for bytecode)
	// at runtime.
	u16 ins;

	/* IC is used only for printing debug info
	* so there is nothing wrong about it being
	* long long (64 bit)*/
	unsigned long long ic;
} cpu;
