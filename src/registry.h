#ifndef u16
#define u16 unsigned short
#endif

#ifndef _REG_H_
#define _REG_H_

struct cpu init_cpu();

void print_cpu_state(struct cpu *_cpu);
void print_memory(struct cpu *_cpu); 

void reset_cpu_state(struct cpu *_cpu);
void free_cpu(struct cpu *_cpu);

#endif
