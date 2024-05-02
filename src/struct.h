#ifndef u16
#define u16 unsigned short
#endif

#define STACK_SIZE 512
#define RAM_SIZE 65535

enum INSTRUCTION {
	INS_MOV,

	INS_ADD,
	INS_SUB,

	INS_INC,
	INS_DEC,

	INS_PUSH,
	INS_POP,

	INS_OR,
	INS_AND,
	INS_XOR,

	INS_JMP,
	INS_HALT,
	INS_NOP,

	INS_END,
};

typedef enum REGISTRY {
	REG_RAX,
	REG_RBX,
	REG_RDX,
	REG_A1,
	REG_A2,
	REG_A3,
	REG_INS,
} REGISTRY;

typedef struct s_cmd {
	enum INSTRUCTION ins;
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
} cmd;

typedef struct s_ins_pool {
	cmd *cmds;
	u16 count;
	u16 capacity;
} instruction_pool;

typedef struct s_code_block {
	instruction_pool ins;
	char *label;
} code_block;

typedef struct s_code_blocks {
	code_block *block;
	u16 count;
	u16 capacity;
} code_blocks;

typedef struct s_ins_pointer {
	code_block *block;
	u16 ins;
} instruction_pointer;

typedef struct ram {
	u16 *cells;
	u16 capacity;
} ram;

typedef struct cpu {
	ram ram;
	instruction_pointer ip;
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
