build:
	gcc -O3 -pedantic -ggdb -Wall -Wextra -o build/asmexe src/lexer.c src/registry.c src/vm.c src/instruction.c
