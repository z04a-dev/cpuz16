#ifndef _STRUCT_IMPL
#include "struct.h"
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <time.h>

#ifndef _SOCKET_IMPL
#include "socket.h"
#endif

#include "util/to_str.h"
#include "util/comp.h"
#include "instruction.h"

#define SECS_TO_NS(SECS) (SECS * 1000000000.)

#define DEBUG 0 // Print instructions.
#define DEBUG_NOOP 0 // Don't execute instructions. (wouldn't HALT)

/* Clock works up to 3.335 mHz max. (After migrating to timerfd, maybe more, NOT TESTED) */
/* Comment HZ to run at full-speed */
/* UPD: Actually target something like 100kHz, that way you won't miss timerfd most of the time */
// #define HZ 3335000.
// #define HZ 1875000.
// #define HZ 100000.
// #define HZ 4210.
// #define HZ 30.

#define CLOCK SECS_TO_NS((1. / HZ))

static u16 *vm_create_bytearray() {
	unsigned int byte_array_size = (NUM_CELLS) * sizeof(u16);
	if (!check_array_size(byte_array_size)) {
		printf("[PANIC] Failed to allocate byte array!\n");
		exit(1);
	}
	printf("[BYTECODE] Allocating byte array<%d bytes>...\n", byte_array_size); 

	/* Using calloc, so array gets initialied with 0 */
	u16 *array = calloc(NUM_CELLS, sizeof(u16));
	if (array == NULL) {
		printf("[PANIC] Failed to allocate bytecode\n");
		printf("[PANIC] Are you out of memory? calloc() failed!\n");
		exit(1);
	}
	return array;
}

u16 *read_binary(char *file) {

	FILE *fp;
	fp = fopen(file, "rb");
	if (fp == NULL) {
		printf("PANIC: Failed to open <%s>...\n", file);
		exit(1);
	}
	u16 *array = vm_create_bytearray();
	u16 *p = array;
	size_t offset_in_bytes = MAGIC_SIZE * 2; // start after MAGIC
	fseek(fp, offset_in_bytes, 0);
	for (; p < &array[NUM_CELLS]; ++p) {
		if (fread(p, sizeof(u16), 1, fp) != 1)
			break;
	}
	fclose(fp);
	return array;
}

// Yoinked from https://www.geeksforgeeks.org/extract-k-bits-given-position-number/
// Function to extract k bits from p position
// and returns the extracted value as integer
static int bitExtracted(int number, int k, int p)
{
    return (((1 << k) - 1) & (number >> (p - 1)));
}

static struct instruction _recognize_ins(instruction_set *_isa, u16 _token) {
	assert(_isa->count > 0 && "initialize ISA first!");
	for (unsigned int i = 0; i < _isa->count; ++i) {
		if (_token == _isa->ins[i].opcode) {
			return _isa->ins[i];
		}
	}	
	return _isa->ins[0]; /* return nop */
}

static bool if_jmp(struct instruction instr) {
	switch (instr.opcode) {
		case 0x0006:
			return true;
			break;
		case 0x0007:
			return true;
			break;
		// Isn't compatible with -Wpedantic
		// case 0x0010 ... 0x0015:
		// 	return true;
		// 	break;
	}
	if (instr.opcode >= 0x0010 && instr.opcode <= 0x0015)
		return true;
	return false;
}

bool execute_instr(instruction_set *_isa, cpu *_cpu) {
	u16 i = _cpu->ins;
	u16 hint_count = 0;
	u16 cell = _cpu->bus.cells[i];

	cmd _cmd = {0};
	struct instruction instr = _recognize_ins(_isa, cell & 0x001F); // Get 5 LSB
	_cmd.ins = instr;
	if (DEBUG)
		printf("FULL [0x%04x] 0x%016b | INSTR: %s |", (unsigned int)i, cell, instr.token);
	u16 val1 = bitExtracted(cell, 3, 6);
	if (val1 == 0x07) {
		hint_count++;
		val1 = _cpu->bus.cells[i+hint_count];
		_cmd.val1_type = T_VAL1_U16;
		_cmd.val1.num = val1;
		if (DEBUG)
			printf(" VAL1: %d |", val1);
	} else {
		_cmd.val1_type = T_VAL1_REG;
		_cmd.val1.reg = val1;
		if (DEBUG)
			printf(" VAL1: %s |", reg_to_str(val1));
	}

	u16 val2 = bitExtracted(cell, 3, 9);

	if (val2 == 0x07) {
		hint_count++;
		val2 = _cpu->bus.cells[i+hint_count];
		_cmd.val2_type = T_VAL2_U16;
		_cmd.val2.num = val2;
		if (DEBUG)
			printf(" VAL2: %d |", val2);
	} else {
		_cmd.val2_type = T_VAL2_REG;
		_cmd.val2.reg = val2;
		if (DEBUG)
			printf(" VAL2: %s |", reg_to_str(val2));
	}

	u16 val3 = bitExtracted(cell, 3, 12);
	if (val3 == 0x07) {
		hint_count++;
		val3 = _cpu->bus.cells[i+hint_count];
		_cmd.val3_type = T_VAL3_U16;
		_cmd.val3.num = val3;
		if (DEBUG)
			printf(" VAL3: %d | TOTAL: %llu\n", val3, _cpu->ic);
	} else {
		_cmd.val3_type = T_VAL3_REG;
		_cmd.val3.reg = val3;
		if (DEBUG)
			printf(" VAL3: %s | TOTAL: %llu\n", reg_to_str(val3), _cpu->ic);
	}

	// printf("FULL [0x%04x]: 0x%016b | INSTR: %s | VAL1: 0x%03b | VAL2: 0x%03b | VAL3: 0x%03b\n", (unsigned int)i, cell, instr.token, val1, val2, val3);
	if (DEBUG && !DEBUG_NOOP)
		for (size_t hint = 0; hint < hint_count; ++hint) {
			printf("HINT [0x%04x]: FULL: 0x%016b | SKIPPING...\n", (unsigned int)(i + (hint + 1)), _cpu->bus.cells[i + (hint + 1)]);
		}

	if (DEBUG_NOOP)
		_cpu->ins += hint_count + 1;
	if (!DEBUG_NOOP) {
		_cpu->ic++;
		int status = execute_instruction(_cpu, &_cmd, NULL);
			if (status == 2)
				_cpu->ins += hint_count + 1;
			if (status == -1) {
				printf("[PANIC] Something panicked, exiting...\n");
				return 1;
			}

		if (!if_jmp(instr))
			_cpu->ins += hint_count + 1; // move to next instruction if not jumping
	}
	return 0;
}

// Compiler decrements label pos by MAGIC_SIZE
void parse_bytecode(instruction_set *_isa, cpu *_cpu) {
#ifdef HZ
    int timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (timer_fd == -1) {
        perror("timerfd_create");
        exit(EXIT_FAILURE);
    }

    struct itimerspec new_value = {0};
    new_value.it_value.tv_nsec = 1000; // start after 1 microsecond 
    new_value.it_interval.tv_nsec = CLOCK; // clock

    if (timerfd_settime(timer_fd, 0, &new_value, NULL) == -1) {
		printf("[PANIC] Failed to create CPU Clock timerfd\n");
        exit(1);
    }

    int epoll_fd = epoll_create(5);
    if (epoll_fd == -1) {
		printf("[PANIC] Failed to create CPU Clock epoll\n");
        exit(1);
    }

    struct epoll_event ev;
    ev.events = EPOLLIN; 
    ev.data.fd = timer_fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, timer_fd, &ev) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

	printf("[INFO] Running at: %f Hz\n", HZ);

	for (;;) {
        int nfds = epoll_wait(epoll_fd, &ev, 1, -1); 
        if (nfds == -1) {
			printf("[PANIC] Failed CPU Clock.\n");
			exit(1);
        }
		if (ev.events & EPOLLIN) {
            uint64_t expirations;
            ssize_t s = read(timer_fd, &expirations, sizeof(expirations));
            if (s != sizeof(expirations)) {
				printf("[PANIC] Failed CPU Clock.\n");
                exit(1);
            }
			if (_cpu->socket.is_connected)
				socket_checkdata(_cpu);
			if (execute_instr(_isa, _cpu) != 0)
				break;
        }
	}
	close(timer_fd);
    close(epoll_fd);
#else
	for (;;) {
		if (_cpu->socket.is_connected)
			socket_checkdata(_cpu);
		if (execute_instr(_isa, _cpu) != 0)
			break;
	}
#endif
}
