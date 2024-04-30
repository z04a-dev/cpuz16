build:
	gcc -ggdb -Wall -Wextra -o build/asmexe src/lexer.c src/registry.c src/vm.c src/instruction.c
