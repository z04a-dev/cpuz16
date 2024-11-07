# imaginary assembly CPU (16 bit) by z04a 
***CPUZ16***

All registers are unsigned. Supports two modes: interpreter and binary.
Has in-house compiler. In-house terminal is in development.

## Registers:

- rax (16 bit)
- rdx (16 bit)
- rbx (16 bit)
- a1, a2, a3 (16 bit)
- ins (16 bit) <- program counter (for bytecode execution)
- ic (64 bit) <- instruction counter (debugging purposes only)
- stack pointer (points to next available cell)
- IP <- instruction pointer (what will be executed) (only used in interpreter)
- RP <- return pointer (points to jmp+1 in caller function) (only used in interpreter)

## Bus

CPU Bus consists of three sections:

- I/O
    - Starts at 0x0000
    - Ends at 0x003F
- RAM
    - Starts at 0x0040
    - Ends at 0x3FFF
- ROM
    - Starts at 0x4000
    - Ends at 0x8000

At the end of RAM, 256 cells are allocated for stack.

## instructions
- #0000 nop <- do nothing
- #0001 mov reg1, what <- put something from what to reg1
- #0002 add reg1, howmuch <- add howmuch to reg1
- #0003 sub reg1, howmuch <- remove howmuch from reg1
- #0004 inc reg1 <- reg1 += 1
- #0005 dec reg1 <- reg1 -= 1
- #0006 jmp where <- jumps at label (don't save return pointer)
- #0007 call where <- calls function (saves return pointer)
- #0008 ret <- returns from function to rp
- #000a and reg1, (reg2 or value) <- bitwise AND (result stored in reg1)
- #000b xor reg1, (reg2 or value) <- bitwise XOR (result stored in reg1)
- #000c push reg1 <- push register to stack
- #000d pop reg1 <- pops value from stack to register
- #0009 or reg1, (reg2 or value) <- bitwise OR (result stored in reg1)
- #000e halt <- panic (stop executing)
- #000f end <- closes code block

***conditional jump ISA extension***

all of instructions receive three args:

<REG || NUM> <REG || NUM> LABEL 
- #0010 jeq <- if first arg == second arg, then jump
- #0011 jne <- if first arg != second arg, then jump
- #0012 jgt <- if first arg >  second arg, then jump
- #0013 jlt <- if first arg <  second arg, then jump
- #0014 jge <- if first arg >= second arg, then jump
- #0015 jle <- if first arg <= second arg, then jump

***memory access ISA extension***
- #0016 lv <- load value from arg2(mem addr) to arg1(reg)
- #0017 sv <- put value of arg2 to arg1(pointer to mem)

***mathematical ISA extension***
- #0018 mul <- multiply val1 by val2
- #0019 div <- divide val1 by val2

***bitshift ISA extension***
- #0018 ror <- rotate right reg1 by val2
- #0019 rol <- rotate left reg1 by val2

## VM

This repository provides you with working in-house implementation of CPUZ16 virtual machine
and CPUZ16 compiler.

To compile them, use `make` command in root directory.

This will create `z16` (VM) and `z16c` (compiler) in build/ directory.

`z16` accepts two types of files: correct .asm and correct CPUZ16 binaries (typically .bin).

There is a number of working (and non working) tests in tests/ directory, so you can try to run them.

## Compiler and bytecode

CPUZ16 has in-house compiler called z16c, that is being compiled with all targets.

If you only want to compile compiler, then use this command:

`make compiler`             <- for optimized version

`make compiler_unoptimized` <- for unoptimized version

To use compiler, simply provide it with correct .asm file, like this:

`z16c ./source.asm`

Compiler will generate .bin file, that will contain correct bytecode, which can be executed using
CPUZ16 VM.

`z16 ./output.bin`

Note:
It's only possible to do nested subroutine calls using bytecode execution.
Raw interpreter would not work.

## Value types

z16 supports hexadecimal and decimal values (binary not supported yet):
- Hexadecimal values are prefixed with `$` (limited to $FFFF)

    `mov a1, $beef;`

- Decimal values are not prefixed. (limited to 65535)

    `mov a1, 50;`

## CPUZ16 preprocessor

Currently z16 supports primitive preprocessed define macros.

Define types:
- IMM (Immediate value) 

    Returns decimal u16 value

    `@EXAMPLE imm = 5;`

    `@EXAMPLE imm = $BEEF;`

- ASCII or ASCIIZ (sequence of bytes (asciiz is null-terminated))

    Returns memory address of first element.

    `@EXAMPLE ascii = "Hello, World!";`

    `@EXAMPLE asciiz = "Hello, World!";`

- DATA (sequence of u16 values)

    Returns memory address of first element.

    `@EXAMPLE data = {$FAFA, 5050, $BEEF};`

To interact with defined value in your code -> use `@` prefix.

    `mov a1, @EXAMPLE;`

## I/O

Currently z16 spawns `socket.sock` in `pwd` directory, which can be used for IPC.

Predefined ports are:
- `0x0000` Write port
- `0x0001` Input port
- `0x0002` Count port (how many terminals are connected)

Of course you can define your own ports in `src/socket.c`.

To interact with `socket.sock` you will need special program (will be released soon).

If you want to try it out now, please message me @z04a-dev.

## Code blocks

every .asm requires start: entry point

you can define 65535 code blocks (including start:)

it's possible to jump between code blocks using jmp 

HOWTO:

```
start:
;; some code
    jmp example;
;; if you don't put HALT in .start, VM will put it automatically at the end.
end;

example:
;; another code block
;; program will halt at the end of this fn, if you don't put RET here.
;; but only if your VM is in interpreter mode, otherwise it will pass through.
end;
```

## Example .asm (old, needs fixing)

```
;; comment
start:
    nop;

    add rax, 1;
    add rbx, 10;
    add rax, rbx;
    ;; Address evaluation is deprecated.
    ;; #VALUE represents hex value now.
    mov rax, #41FA;

    add a3, 500;
    push a3;
    
    sub a3, 1712;
    pop a3;

    inc a1;
    xor a1, a3;
    inc a1;
    jmp fn;

    halt;
end;

fn:
    inc rax;
    inc rax;
    sub rax, 7;
    ret;
end;

```

## Contribution

Pull requests are welcome, although this project is simply for learning purposes,
so it can be abandoned!

