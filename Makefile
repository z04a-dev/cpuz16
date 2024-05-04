build:
	gcc -O3 -pedantic -ggdb -Wall -Wextra -o build/asmexe src/isa.c src/lexer.c src/registry.c src/vm.c src/instruction.c
unoptimized:
	gcc -ggdb -Wall -Wextra -o build/asmexe src/isa.c src/lexer.c src/registry.c src/vm.c src/instruction.c
