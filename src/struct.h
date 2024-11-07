#include <sys/epoll.h>
#ifndef u16
#define u16 unsigned short
#endif

#include <stdio.h>
#include <sys/socket.h>
#include <sys/un.h>

#define _STRUCT_IMPL

#include <stdbool.h>
#define BUS_SIZE 32768

#define STACK_SIZE 256
#define IO_SIZE 64

#define RAM_SIZE 16384 - IO_SIZE
#define RAM_START IO_SIZE

#define ROM_START RAM_START + RAM_SIZE
// TODO it doesn't calculate ROM_SIZE correctly for some reason
// #define ROM_SIZE BUS_SIZE - ROM_START
#define ROM_SIZE 16384

#define BUF_EVS 10

// 8bit I\O
#define CONNECTIONS 5      // num of terminals
#define OUTPUT_PORT 0x0000 // port which receives data from cpu, sends it to socket
#define INPUT_PORT  0x0001 // port which receives data from socket, sends it to cpu
#define COUNT_PORT  0x0002 // how many devices connected

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

// TODO
// Can i drop numbers in enum? T_VAL1_NULL -> T_VAL_NULL
// that will make everything easier
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
} bus;

#define return_pointer instruction_pointer

typedef enum {
	VM_BINARY,
	VM_INTERPRETER
} state;

typedef struct epl {
	int plfd;
	struct epoll_event ev;
	struct epoll_event evs[BUF_EVS];
} epl;

typedef struct z16_socket {
	char *sock_path;
	bool is_connected;
	int server_socket;
	int client_socket;
	struct sockaddr_un server_addr;
	struct sockaddr_un client_addr;
	socklen_t clen;
	epl epoll;
} z16_socket;

typedef struct cpu {
	// VM stuff
	z16_socket socket;
	state state;
	// cpu
	bus bus;
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
	u16 ins;

	/* IC is used only for printing debug info
	* so there is nothing wrong about it being
	* long long (64 bit)*/
	unsigned long long ic;
} cpu;
