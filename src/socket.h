#ifndef _STRUCT_IMPL
#include "struct.h"
#endif

#define _SOCKET_IMPL

int socket_init(cpu *_cpu, char *path); 

void socket_checkdata(cpu *_cpu);
void socket_write(cpu *_cpu, u16 addr, u16 val); 
