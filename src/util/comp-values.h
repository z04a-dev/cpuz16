#ifndef _UTIL_COMPILER_VALUES
#define _UTIL_COMPILER_VALUES
#endif

#define NUM_CELLS 16384 /* there is 16384 cells of 16 bit values available in cpuz16 */
#define MAGIC_SIZE 1 /* 1 magic cell would be enough */
/* there is no support for MAGIC sizes > 1 */

#define TOTAL_CELLS NUM_CELLS+MAGIC_SIZE

#define MAX_ROM_SIZE 32768 /* CPUZ16 can't load more than 32 KiB of ROM */

#define MAGIC_VALUE 0xFF3A

#define BYTECODE_START MAGIC_SIZE /* code will start at array[MAGIC_SIZE], which will put it right after magic */

#define SWAP_ENDIANNESS false 
