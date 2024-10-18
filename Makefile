build: compiler vm
unoptimized: compiler_unoptimized vm_unoptimized

compiler:
	gcc -O3 -pedantic -ggdb -Wall -Wextra -o build/z16c src/lexer.c src/isa.c src/compiler.c

vm:
	gcc -O3 -pedantic -ggdb -Wall -Wextra -DVM_BUILD -o build/z16 src/isa.c src/lexer.c src/registry.c src/vm.c src/instruction.c

compiler_unoptimized:
	gcc -ggdb -Wall -Wextra -o build/z16c src/lexer.c src/isa.c src/compiler.c

vm_unoptimized:
	gcc -ggdb -Wall -Wextra -DVM_BUILD -o build/asmexe src/isa.c src/lexer.c src/registry.c src/vm.c src/instruction.c

test:
	./test.sh
