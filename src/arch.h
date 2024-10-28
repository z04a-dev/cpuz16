/* registry */
#define _ARCH_IMPL

#define RAX_REGISTRY 0x00 // 0b000
#define RBX_REGISTRY 0x01 // 0b001
#define RDX_REGISTRY 0x02 // 0b010
#define A1_REGISTRY  0x03 // 0b011
#define A2_REGISTRY  0x04 // 0b100
#define A3_REGISTRY  0x05 // 0b101
#define INS_REGISTRY 0x06 // 0b110
#define HINT_FLAG    0x07 // 0b111

// #define RAX_REGISTRY 0b000
// #define RBX_REGISTRY 0b001
// #define RDX_REGISTRY 0b010
// #define A1_REGISTRY  0b011
// #define A2_REGISTRY  0b100
// #define A3_REGISTRY  0b101
// #define INS_REGISTRY 0b110
// #define HINT_FLAG    0b111

// #define NOP_OPCODE   0b00000 
// #define MOV_OPCODE   0b00001
// #define ADD_OPCODE   0b00010
// #define SUB_OPCODE   0b00011
// #define INC_OPCODE   0b00100
// #define DEC_OPCODE   0b00101
// #define JMP_OPCODE   0b00110
// #define CALL_OPCODE  0b00111
// #define RET_OPCODE   0b01000
// #define OR_OPCODE    0b01001
// #define AND_OPCODE   0b01010
// #define XOR_OPCODE   0b01011
// #define PUSH_OPCODE  0b01100
// #define POP_OPCODE   0b01101
// #define HALT_OPCODE  0b01110
// #define END_OPCODE   0b01111
// #define JEQ_OPCODE   0b10000
// #define JNE_OPCODE   0b10001
// #define JGT_OPCODE   0b10010
// #define JLT_OPCODE   0b10011
// #define JGE_OPCODE   0b10100
// #define JLE_OPCODE   0b10101
// #define LV_OPCODE    0b10110
// #define SV_OPCODE    0b10111
// #define MUL_OPCODE   0b11000
// #define DIV_OPCODE   0b11001
/* opcodes */

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

/* bitshifting ISA extension */
#define ROR_OPCODE 0x001a
#define ROL_OPCODE 0x001b
