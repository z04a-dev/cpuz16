#define NOP_OPCODE   0x0000
#define MOV_OPCODE   0x0001

#define ADD_OPCODE   0x0002
#define SUB_OPCODE   0x0003

#define INC_OPCODE   0x0004
#define DEC_OPCODE   0x0005

#define JMP_OPCODE   0x0006
#define CALL_OPCODE  0x0007
#define RET_OPCODE   0x0008

#define OR_OPCODE    0x0009
#define AND_OPCODE   0x000a
#define XOR_OPCODE   0x000b

#define PUSH_OPCODE  0x000c
#define POP_OPCODE   0x000d

#define HALT_OPCODE  0x000e
#define END_OPCODE   0x000f

/* conditional jmp ISA extension */
#define JEQ_OPCODE 0x0010
#define JNE_OPCODE 0x0011
#define JGT_OPCODE 0x0012
#define JLT_OPCODE 0x0013
#define JGE_OPCODE 0x0014
#define JLE_OPCODE 0x0015

/* memory access ISA extension */
#define LV_OPCODE 0x0016
#define SV_OPCODE 0x0017

/* math ISA extension */
#define MUL_OPCODE 0x0018
#define DIV_OPCODE 0x0019
