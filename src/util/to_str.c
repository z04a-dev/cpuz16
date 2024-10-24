
#ifndef _STRUCT_IMPL
#include "../struct.h"
#endif 

#ifndef _ARCH_IMPL
#include "../arch.h"
#endif
char *reg_to_str(u16 reg) {
	switch (reg) {
		case RAX_REGISTRY:
			return "rax";
			break;
		case RBX_REGISTRY:
			return "rbx";
			break;
		case RDX_REGISTRY:
			return "rdx";
			break;
		case A1_REGISTRY:
			return "a1";
			break;
		case A2_REGISTRY:
			return "a2";
			break;
		case A3_REGISTRY:
			return "a3";
			break;
		case INS_REGISTRY:
			return "ins";
			break;
		case HINT_FLAG:
			return "hint";
			break;
	}
	return "";
}
