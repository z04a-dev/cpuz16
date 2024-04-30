#ifndef u16
#define u16 unsigned short
#endif

typedef struct cpu {
	u16 rax;
	u16 rbx;
	u16 rdx;
	u16 a1;
	u16 a2;
	u16 a3;
	u16 ins;
} cpu;

enum INSTRUCTION {
	INS_NOP,
	INS_JMP,
	INS_ADD,
	INS_SUB,
	INS_INC,
	INS_DEC,
	INS_HALT,
};

enum REGISTRY {
	REG_RAX,
	REG_RBX,
	REG_RDX,
	REG_A1,
	REG_A2,
	REG_A3,
	REG_INS,
};

typedef struct s_cmd {
	enum INSTRUCTION ins;
	enum{ T_VAL1_U16, T_VAL1_REG } val1_type;
	union {
		u16 num;
		enum REGISTRY reg;
	} val1;
	enum{ T_VAL2_U16, T_VAL2_REG } val2_type;
	union {
		u16 num;
		enum REGISTRY reg;
	} val2;
} cmd;


