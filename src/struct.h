#ifndef u16
#define u16 unsigned short
#endif

#define STACK_SIZE 512
#define RAM_SIZE 65535

typedef struct instruction {
	u16 opcode;
	const char *token;
} ins;

typedef struct instruction_set {
	ins *ins;
	unsigned int count;
} instruction_set;

#define CLITERAL(type) (type) /* yoinked from raylib */

typedef enum REGISTRY {
	REG_RAX,
	REG_RBX,
	REG_RDX,
	REG_A1,
	REG_A2,
	REG_A3,
	REG_INS,
} REGISTRY;

typedef struct {
	ins ins;
	enum{ T_VAL1_U16, T_VAL1_REG, T_VAL1_LABEL, T_VAL1_ADDRESS } val1_type;
	union {
		u16 num;
		enum REGISTRY reg;
		char *label;
	} val1;
	enum{ T_VAL2_U16, T_VAL2_REG, T_VAL2_LABEL, T_VAL2_ADDRESS } val2_type;
	union {
		u16 num;
		enum REGISTRY reg;
		char *label;
	} val2;
	enum{ T_VAL3_U16, T_VAL3_REG, T_VAL3_LABEL, T_VAL3_ADDRESS } val3_type;
	union {
		u16 num;
		enum REGISTRY reg;
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

typedef struct cpu {
	instruction_set isa;
	ram ram;
	instruction_pointer ip;
	return_pointer rp;
	u16 *stack_value;
	u16 *stack_pointer;
	u16 rax;
	u16 rbx;
	u16 rdx;
	u16 a1;
	u16 a2;
	u16 a3;
	u16 ins;
} cpu;
