build: compiler vm
unoptimized: compiler_unoptimized vm_unoptimized

compiler:
	gcc -std=gnu23 -O3 -pedantic -ggdb -Wall -Wextra -o build/z16c src/parser.c src/isa.c src/compiler.c

vm:
	gcc -std=gnu23 -O3 -pedantic -ggdb -Wall -Wextra -DVM_BUILD -o build/z16 src/isa.c src/parser.c src/bytecode.c src/util/to_str.c src/registry.c src/vm.c src/instruction.c

compiler_unoptimized:
	gcc -std=gnu23 -ggdb -Wall -Wextra -o build/z16c src/parser.c src/isa.c src/compiler.c

vm_unoptimized:
	gcc -std=gnu23 -ggdb -Wall -Wextra -DVM_BUILD -o build/asmexe src/isa.c src/parser.c src/bytecode.c src/util/to_str.c src/registry.c src/vm.c src/instruction.c

test:
	./test.sh
